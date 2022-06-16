// Fill out your copyright notice in the Description page of Project Settings.


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
//#include "GameState/SATORI_GameState.h"
#include "SATORIGameMode.h"
#include "GameFramework\CharacterMovementComponent.h"



// Sets default values
ASATORI_AICharacter::ASATORI_AICharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSet"));

	HealthBarWidgetComponen = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponen"));

	//HeadComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Head Position"));

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->SensingInterval = .25f; // 4 times per second
	PawnSensor->bOnlySensePlayers = false;
	PawnSensor->SetPeripheralVisionAngle(85.f);

	// Enemy Stats
	EnemyStatComponent = CreateDefaultSubobject<USATORI_EnemyStatComponent>(TEXT("StatComponent"));

	//bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Spawner/BT_Spawner.BT_Spawner")));
	//btree = bte.LoadSynchronous();

	EnemyType = SATORIEnemyType::None;

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

// Called when the game starts or when spawned
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

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(this);
	if (Character)
	{
		FGameplayTagContainer TagContainer;
		Character->GetOwnedGameplayTags(TagContainer);
		AbilitySystemComponent->AddLooseGameplayTags(TagContainer);
	}
	AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));

	//Needed for targeting system (Nacho)
	if (bIsTargetable) {
		RegisterInTargetableArray();
	}
}

void ASATORI_AICharacter::OnConstruction(const FTransform& Transform)
{
	if (HealthBarWidgetComponen)
	{
		HealthBarWidgetComponen->RegisterComponent();
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		//const APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//UUserWidget* HealthWidget = PC ? CreateWidget(PC/*, HealthBarUI*/) : nullptr;
		//if (HealthWidget)
		//{
		//	HealthBarWidgetComponen->SetWidget(HealthWidget);
		//}
		//HealthBarWidgetComponen->AttachToComponent(HeadComponent, AttachmentRules);
		/*struct ConstructorHelpers::FClassFinder<USATORI_EnemyHealthBar> EnemyUIBar(TEXT("/Game/SATORI/UI/Enemy/"));
		if (EnemyUIBar.Class != NULL)*/
		if (HealthBarUI)
		{
			HealthBarWidgetComponen->SetWidgetSpace(EWidgetSpace::World);
			HealthBarWidgetComponen->SetDrawSize(FVector2D(200.f, 20.f));
			HealthBarWidgetComponen->SetWidgetClass(HealthBarUI);
			FVector2D LocalDrawSize = FVector2D(100.0f, 20.0f);
			HealthBarWidgetComponen->SetDrawSize(LocalDrawSize);
			FVector2D LocalPivot = FVector2D(0.5f, 0.5f);
			HealthBarWidgetComponen->SetPivot(LocalPivot);
		}
	}
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
		GrantAbilityToPlayer(FGameplayAbilitySpec(Ability.SATORIAbility, 1, static_cast<uint32>(Ability.AbilityKeys), this));
	}
}

void ASATORI_AICharacter::GrantAbilityToPlayer(FGameplayAbilitySpec Ability)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!Ability.Ability)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s() Ability Not Granted for %s. Ability is not valid."), *FString(__FUNCTION__), *GetName());
		return;
	}

	AbilitySystemComponent->GiveAbility(Ability);
}


void ASATORI_AICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Display, TEXT("LLEGAMOS A VER SI SE LLAMA A ESTA FUNCION PADRE"));

	

	if (Cast<AAIController>(NewController) != nullptr) {

		
		//AddGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.AI"));
		Tags.Add("PossessedBy.AI");

		UE_LOG(LogTemp, Display, TEXT("EJECUTAMOS EL BT"));

		AAIController* controller = Cast<AAIController>(NewController);

		AttributeSetBase = AttributeSet;
		btree = bte.LoadSynchronous();
		controller->RunBehaviorTree(btree);

		InitializeAttributes();
		AddAICharacterAbilities();
		SetHealth(GetMaxHealth());

	}

}

float ASATORI_AICharacter::getDistAttack()
{
	return dist_attack;
}

float ASATORI_AICharacter::getMaxRangeDist()
{
	return max_range_dist;
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

	FVector delta = EndPosition - StartPosition;
	TWeakObjectPtr<AActor> NewActor;

	bool bHit = false;
	for (int i = -5; i <= 5; i++)
	{
		FVector Axis = FVector::ZAxisVector;
		float rad = FMath::DegreesToRadians(i * 5);
		FQuat quaternion = FQuat(Axis, rad);
		FRotator rotator = FRotator(quaternion);
		FVector newDelta = rotator.RotateVector(delta);

		FVector newEndPos = newDelta + StartPosition;

		bool newHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartPosition,
			newEndPos,
			ECollisionChannel::ECC_Pawn,
			Params
		);

		::DrawDebugLine(World, StartPosition, newEndPos, newHit ? FColor::Green : FColor::Red, false, 1.0f);
		if (newHit)
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
	if(bursting)
	{
		time_burst -= DeltaSeconds;
		if(time_burst <= 0.f)
		{
			bursting = false;
		}
	}

	if (HealthBarUI)
	{
		HealthBarProjection(HealthBarWidgetComponen, 1024, 0.1, 0.5);
		//// HealthBarUI
		//USATORI_EnemyHealthBar* HearthBar = Cast<USATORI_EnemyHealthBar>(HealthBarWidgetComponen->GetWidget());
		//if (HearthBar)
		//{
		//	float Health = EnemyStatComponent->GetHealth();
		//	float Maxhealth = EnemyStatComponent->GetMaxHealth();
		//	HearthBar->UpdateHealthBar(Health, Maxhealth);
		//}
	}
	
	Super::Tick(DeltaSeconds);
}

void ASATORI_AICharacter::sendDamage(float dmg)
{
	if(!bursting)
	{
		time_burst = 5.f;
		dmg_burst = 0.f;
		bursting = true;

	}

	dmg_burst += dmg;


	float max_health_possible = GetMaxHealth();
	UE_LOG(LogTemp, Display, TEXT("La max health es: %f"), max_health_possible);

	if(dmg_burst>= max_health_possible*0.2f)
	{

		ASATORI_CharacterBase* pryeba = Cast<ASATORI_CharacterBase>(this);
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));

		//AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));
	}
	
	float life = GetHealth();

	if(life <=0.0f)
	{
		UE_LOG(LogTemp, Display, TEXT("La vida es: %f"), life);
		ASATORI_Spawned* Spawned = Cast<ASATORI_Spawned>(this);
		

		if (Spawned != nullptr)
		{
			
			//Spawned->Spawner->AddNumEnemies(1);
			float b = Spawned->SpawnedDie();

			
			CharacterDeath();
		}
		else
		{
			CharacterDeath();
		}
		
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

	SetActorEnableCollision(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->Velocity = FVector(0);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		FString Death = "Dying";
		AIController->GetBrainComponent()->StopLogic(Death);
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
}