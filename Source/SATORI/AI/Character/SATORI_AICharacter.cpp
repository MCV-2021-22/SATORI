//

#include "AI/Character/SATORI_AICharacter.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Controller.h"
#include "Data/SATORI_AbilityDataAsset.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "Character/SATORI_PlayerState.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "SATORICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Spawned/SATORI_Spawned.h"
#include "Components/WidgetComponent.h"
#include "UI/Enemy/SATORI_EnemyHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/Components/SATORI_EnemyStatComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SATORIGameMode.h"
#include "GameFramework\CharacterMovementComponent.h"

ASATORI_AICharacter::ASATORI_AICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyType = SATORIEnemyType::None;
	
	AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSet"));

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->SensingInterval = .25f; // 4 times per second
	PawnSensor->bOnlySensePlayers = false;
	PawnSensor->SetPeripheralVisionAngle(85.f);

	// Enemy Stats
	EnemyStatComponent = CreateDefaultSubobject<USATORI_EnemyStatComponent>(TEXT("StatComponent"));

	// Weapon Component
	SwordComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	AttackingCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sword Collision"));
	if (SwordComponent)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponent->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket2");
		// Sphere Collision
		AttackingCollision->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollision->SetCollisionProfileName("Pawn");
		AttackingCollision->SetGenerateOverlapEvents(false);
		AttackingCollision->AttachTo(SwordComponent);
	}

}

void ASATORI_AICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddAICharacterAbilities();

		//Needed for abilities actors (Nacho)
		AddGameplayTag(EnemyTag);
	}

	//Needed for targeting system (Nacho)
	if (bIsTargetable) {
		RegisterInTargetableArray();
	}
}

void ASATORI_AICharacter::OnConstruction(const FTransform& Transform)
{

}

void ASATORI_AICharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ASATORI_AICharacter::AddAICharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAbilities)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAbility for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	for (FSATORIGameplayAbilityInfo Ability : DefaultAbilities->Abilities)
	{
		GrantAbility(FGameplayAbilitySpec(Ability.SATORIAbility, 1, static_cast<uint32>(Ability.AbilityKeys), this));
	}
}

void ASATORI_AICharacter::GrantAbility(FGameplayAbilitySpec Ability)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!Ability.Ability)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Ability Not Granted for %s. Ability is not valid."), *FString(__FUNCTION__), *GetName());
		return;
	}

	AbilitySystemComponent->GiveAbility(Ability);
}


void ASATORI_AICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (Cast<AAIController>(NewController) != nullptr) {

		Tags.Add("PossessedBy.AI");
		AttributeSetBase = AttributeSet;

		AAIController* AIController = Cast<AAIController>(NewController);
	
		if (AIController)
		{
			AIController->RunBehaviorTree(BehaviorTree.LoadSynchronous());
		}

		InitializeAttributes();
		AddAICharacterAbilities();
		SetHealth(GetMaxHealth());
	}
}

float ASATORI_AICharacter::GetAttackDistance()
{
	return AttackDistance;
}

float ASATORI_AICharacter::GetMaxRange()
{
	return MaxRange;
}

bool ASATORI_AICharacter::CheckPlayerWithRayCast()
{
	const FVector StartPosition = GetActorLocation();
	const FRotator StartRotation = GetActorRotation();
	const FVector EndPosition = StartPosition + (StartRotation.Vector() * 300.0f);

	UWorld* World = GetWorld();
	FHitResult HitResult;
	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
	Params.AddIgnoredActor(RootComponent->GetOwner());

	FVector Delta = EndPosition - StartPosition;
	TWeakObjectPtr<AActor> NewActor;

	bool bHit = false;
	for (int i = -5; i <= 5; i++)
	{
		float Rad = FMath::DegreesToRadians(i * 5);
		FRotator Rotator = FRotator(FQuat(FVector::ZAxisVector, Rad));
		FVector newDelta = Rotator.RotateVector(Delta);

		FVector newEndPosition = newDelta + StartPosition;

		bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPosition, newEndPosition, ECollisionChannel::ECC_Pawn, Params);

		if (bDrawDebug)
		{
			DrawDebugLine(World, StartPosition, newEndPosition, Hit ? FColor::Green : FColor::Red, false, 1.0f);
		}

		if (Hit)
		{
			NewActor = HitResult.Actor;
			bHit = true;
			break;
		}
	}

	TWeakObjectPtr<ASATORICharacter> PlayerCharacter = Cast<ASATORICharacter>(HitResult.Actor);
	if (bHit)
	{
		if (PlayerCharacter.IsValid() && EnemyType == SATORIEnemyType::Melee)
		{
			isInFrontPlayer = true;
		}
	}
	else
	{
		isInFrontPlayer = false;
	}
	return isInFrontPlayer;
}

void ASATORI_AICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASATORI_AICharacter::CheckDamage()
{
	//Death
	if(GetHealth() <= 0.0f)
	{
		//Spawned edge case
		ASATORI_Spawned* Spawned = Cast<ASATORI_Spawned>(this);
		if (Spawned != nullptr)
		{
			Spawned->SpawnedDie();
		}

		AddGameplayTag(DeadTag);
	}
}

void ASATORI_AICharacter::HealthBarProjection(UWidgetComponent* HealthBar, float ViewDistance, float RangeA, float RangeB)
{
	ASATORICharacter* EnemyCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector PlayerLocation = FVector(0.0f);
	FVector SelfLocation = FVector(0.0f);
	FVector WidgetLocation = FVector(0.0f);

	if (EnemyCharacter)
	{
		// Widget Size
		PlayerLocation = EnemyCharacter->GetActorLocation();
		SelfLocation = GetActorLocation();
		FVector ResultVector = PlayerLocation - SelfLocation;
		float vectorLength = ResultVector.Size();
		float result = UKismetMathLibrary::MapRangeClamped(vectorLength, 0, ViewDistance, RangeB, RangeA);

		FVector ScaleVector(result);
		HealthBar->SetWorldScale3D(ScaleVector);
	}

	if (GetWorld())
	{
		// Widget Rotation
		WidgetLocation = HealthBar->GetComponentLocation();
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			USceneComponent* LocalTransform = CameraManager->GetTransformComponent();
			FVector CameraLocation = LocalTransform->GetComponentLocation();
			FRotator ResultRotator = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
			HealthBar->SetWorldRotation(ResultRotator);
		}

	}
}
//Target system (Nacho)
bool ASATORI_AICharacter::IsTargetable_Implementation() const
{
	return bIsTargetable;
}

void ASATORI_AICharacter::RegisterInTargetableArray_Implementation()
{
	if (bIsTargetable)
	{
		GetWorld()->GetAuthGameMode<ASATORIGameMode>()->AddEnemyActor(this);
		//GetWorld()->GetGameState<ASATORI_GameState>()->AddEnemyActor(this);
	}
}

void ASATORI_AICharacter::CharacterDeath()
{
	RemoveCharacterAbilities();

	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(this);
	//GetWorld()->GetGameState<ASATORI_GameState>()->RemoveEnemyActor(this);

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//SetActorEnableCollision(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->Velocity = FVector(0);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		FString Death = "Dying";
		UBrainComponent* BrainComponent = AIController->GetBrainComponent();
		if(BrainComponent)
		{ 
			AIController->GetBrainComponent()->StopLogic(Death);
		}
	}
	
	// Apply Gold to player
	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter && GoldRewardGameplayEffect)
	{
		PlayerCharacter->ApplyGameplayeEffectToPlayerWithParam(GoldRewardGameplayEffect);
	}

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);

	}

	if (DeathMontage)
	{
		DeathMontage->bEnableAutoBlendOut = false;
		float TimeToEnd = PlayAnimMontage(DeathMontage);
		FTimerHandle TimerHandleDestroy;
		GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_AICharacter::DestroyMyself, TimeToEnd, false);

	}
	else
	{
		DestroyMyself();
	}

}

void ASATORI_AICharacter::DestroyMyself()
{
	Destroy();
}

void ASATORI_AICharacter::RemoveCharacterAbilities()
{
	AbilitySystemComponent->BlockAbilitiesWithTags(BlockTags);

	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

	if (RemovedgameplayAbilities.Num() > 0)
	{
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && RemovedgameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}
}