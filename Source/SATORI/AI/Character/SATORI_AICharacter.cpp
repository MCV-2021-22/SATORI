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
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "AI/Character/Melee/SATORI_Melee.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Actors/Coin/SATORI_CoinRewardActor.h"
#include "Components/Player/SATORI_StatsComponent.h"
#include "AIController.h"

ASATORI_AICharacter::ASATORI_AICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyType = SATORIEnemyType::None;
	EnemyImpactType = SATORIEnemyImpactType::None;

	AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSet"));

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->SensingInterval = .25f; // 4 times per second
	PawnSensor->bOnlySensePlayers = false;
	PawnSensor->SetPeripheralVisionAngle(85.f);

	// Enemy Stats
	EnemyStatComponent = CreateDefaultSubobject<USATORI_EnemyStatComponent>(TEXT("StatComponent"));

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

	USkeletalMeshComponent* CharacterMesh = this->GetMesh();
	TArray<UMaterialInterface*> Materials= CharacterMesh->GetMaterials();
	
	if (Materials.Num() > 0)
	{
		for (int i = 0; i < Materials.Num(); i++)
		{
			UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(Materials[i], this);
			DynamicMaterials.Add(Material);
			CharacterMesh->SetMaterial(i, DynamicMaterials[i]);
		}
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

		ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character)
		{
			USATORI_StatsComponent* StatComponent = Character->GetStatsComponent();
			if (StatComponent)
			{
				StatComponent->FOnPlayerDeathBroadCast.AddDynamic(this, &ASATORI_AICharacter::CheckPlayerIsDeathState);
			}
		}
	}

	if (NewController)
	{
		EnemyAIController = Cast<AAIController>(NewController);
	}
}

float ASATORI_AICharacter::GetAttackDistance()
{
	return AttackDistance;
}

void ASATORI_AICharacter::StopAILogic()
{
	if (EnemyAIController)
	{
		UBrainComponent* AIBrain = EnemyAIController->BrainComponent;
		if (AIBrain)
		{
			AIBrain->StopLogic("PlayerDead");
		}
	}
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

	if (bursting)
	{
		time_burst -= DeltaSeconds;
		if (time_burst <= 0.f)
		{
			bursting = false;
			RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));
		}
	}
	if(canDestroy)
	{
		/*
		FTimerHandle WaitHandle;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				Destroy();
			}), 2.0f, false);
		GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
		*/

		Destroy();

		
	}

}

void ASATORI_AICharacter::CheckDamage(float Damage)
{
	//Melee
	ASATORI_Melee* Melee = Cast<ASATORI_Melee>(this);
	if (Melee)
	{
		for (int count = 0; count < Damage / DamageCounter; count++)
		{
			USATORI_BlueprintLibrary::ApplyGameplayEffect(this, CountGameplayEffect);
		}
	}
	else
	{

		int prob_block = rand() % 100;
		if(prob_block < 90)
		{
		dmg_burst += Damage;
		}

		if (!bursting)
		{
			time_burst = 5.f;
			dmg_burst = 0.f;
			bursting = true;
		}
		
		float max_health_possible = GetMaxHealth();
		UE_LOG(LogTemp, Display, TEXT("La max health es: %f"), max_health_possible);
		UE_LOG(LogTemp, Display, TEXT("Damage dealt: %f"), Damage);
		UE_LOG(LogTemp, Display, TEXT("Damage burst: %f"), dmg_burst);

		if (dmg_burst >= max_health_possible * 0.2f)
		{

			ASATORI_CharacterBase* pryeba = Cast<ASATORI_CharacterBase>(this);
			if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("State.Burst"))))
			{
				AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));
			}
			//AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));

		}
	}

	//USATORI_BlueprintLibrary::ApplyGameplayEffect(this, HitGameplayEffect);

	//Death

	if (GetHealth() <= GetMaxHealth() * 0.75 && HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Inicial"))))
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(this);
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(this);

		if(Fujin)
		{
			Fujin->Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final")));
			Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final")));

			Fujin->Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Fase.Inicial"));
			Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Fase.Inicial"));

			Fujin->Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Raijin"));

		}
		else if(Raijin)
		{
			Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final")));
			Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final")));

			Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Fase.Inicial"));
			Raijin->Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Fase.Inicial"));

			Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Fujin"));
		}
	}

	if(GetHealth() <= GetMaxHealth()*0.25)
	{
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(this);
		if(Raijin)
		{
			Raijin->setNumRayosExtensibles(6);
		}
		
	}
	else if(GetHealth() <= GetMaxHealth()*0.5)
	{
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(this);
		if (Raijin)
		{
			Raijin->setNumRayosExtensibles(5);
		}
		
	}
	else if(GetHealth() <= GetMaxHealth()* 0.75)
	{
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(this);
		if (Raijin)
		{
			Raijin->setNumRayosExtensibles(4);
		}
	}

	if(GetHealth() <= 0.0f)
	{
		//Spawned edge case
		ASATORI_Spawned* Spawned = Cast<ASATORI_Spawned>(this);
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(this);
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(this);
		

		//Melee
		if (Melee)
		{
			AddGameplayTag(DeadTag);
		}
		else if(Fujin)
		{
			if(Fujin->getRaijinDowned())
			{
				Fujin->setDead(true);
				Fujin->Raijin->setDead(true);
				AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
				Fujin->Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
			}
			else
			{
				AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Downed"));
				RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Jugable"));

			}
		}
		else if(Raijin)
		{
			if (Raijin->getFujinDowned())
			{
				Raijin->setDowned(false);
				Raijin->Fujin->setDowned(false);

				Raijin->setDead(true);
				Raijin->Fujin->setDead(true);
				AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
				Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
			}
			else
			{
				AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Downed"));
				RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Jugable"));
			}
		}
		else if (Spawned)
		{
			Spawned->SpawnedDie();
		}
		else
		{
			AActor* Actor = Cast<AActor>(this);

			AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
			//GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor);
			//Destroy();
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
	}
}

void ASATORI_AICharacter::CharacterDeath()
{
	
}

void ASATORI_AICharacter::CheckImpactReceivedByPlayer(EComboState State)
{
	switch (State)
	{
	case EComboState::None:
		EnemyImpactType = SATORIEnemyImpactType::None;
		break;
	case EComboState::LightAttack_1:
		EnemyImpactType = SATORIEnemyImpactType::LightAttack1;
		break;
	case EComboState::LightAttack_2:
		EnemyImpactType = SATORIEnemyImpactType::LightAttack2;
		break;
	case EComboState::LightAttack_3:
		EnemyImpactType = SATORIEnemyImpactType::LightAttack3;
		break;
	case EComboState::HeavyAttack:
		EnemyImpactType = SATORIEnemyImpactType::HeavyAttack;
		break;
	default: break;
	}
}

void ASATORI_AICharacter::SetDamagedColor()
{
	bool HasBlckTag = this->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName("State.Special")));

	bool IsBossInmmune = this->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Inmune"));

	if (!HasBlckTag || !IsBossInmmune)
	{
		for (int i = 0; i < DynamicMaterials.Num(); i++)
		{
			if (DynamicMaterials[i])
			{
				DynamicMaterials[i]->SetScalarParameterValue(FName(TEXT("BaseColor")), 1.0f);
			}
		}

		FTimerHandle WaitHandle;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, [this]()
			{
				for (int i = 0; i < DynamicMaterials.Num(); i++)
				{
					if (DynamicMaterials[i])
					{
						DynamicMaterials[i]->SetScalarParameterValue(FName(TEXT("BaseColor")), 0.0f);
					}
				}
			}, 0.1f, false);
	}
}

void ASATORI_AICharacter::SpawnCointActorAfterDeath()
{
	FActorSpawnParameters SpawnParams;
	if (SpawnCoinActor.Get())
	{
		ASATORI_CoinRewardActor* SpawnedActorRef = GetWorld()->SpawnActor<ASATORI_CoinRewardActor>(SpawnCoinActor, 
			this->GetActorTransform(), SpawnParams);
	}
}
void ASATORI_AICharacter::EnemyDissolveAfterDeath(float Timer)
{
	if (DynamicMaterials.Num() > 0)
	{
		TimeCountDown = Timer;
		GetWorld()->GetTimerManager().SetTimer(MaterialWaitHandle, [this]()
			{
				for (int i = 0; i < DynamicMaterials.Num(); i++)
				{
					DynamicMaterials[i]->SetScalarParameterValue(FName(TEXT("Appearance")), TimeCountDown);
					TimeCountDown -= LocalRate;
					if (TimeCountDown <= 0)
					{
						if (CheckPlayerDeath)
						{
							this->Destroy();
							break;
						}
						DynamicMaterials[i]->SetScalarParameterValue(FName(TEXT("Appearance")), -0.1f);
						GetWorld()->GetTimerManager().ClearTimer(MaterialWaitHandle);
					}
				}
			}, LocalRate, true, 0.0f);
	}
}

void ASATORI_AICharacter::CheckPlayerIsDeathState(bool isDeath)
{
	CheckPlayerDeath = isDeath;
}