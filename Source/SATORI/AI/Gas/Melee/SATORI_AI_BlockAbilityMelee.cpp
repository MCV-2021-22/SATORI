//

#include "AI/Gas/Melee/SATORI_AI_BlockAbilityMelee.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

USATORI_AI_BlockAbilityMelee::USATORI_AI_BlockAbilityMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bBlocking = false;
	bIsCreateOnRunning = GIsRunning;
}

void USATORI_AI_BlockAbilityMelee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	bBlocking = false;

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_AI_BlockAbilityMelee: Cannot get Animation Montage ... "), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Melee = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	if (!Melee)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_BlockAbilityMelee: Cannot Cast ASATORICharacter ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	RotationRate = Melee->GetCharacterMovement()->RotationRate.Yaw;
	GetTarget();

	if (!TagSpawnAbility.IsValid()  || !BlockDamageTag.IsValid() || !CanBeStunnedTag.IsValid() || !BlockingTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_AI_BlockAbilityMelee: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AI_BlockAbilityMelee::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AI_BlockAbilityMelee::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AI_BlockAbilityMelee::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AI_BlockAbilityMelee::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AI_BlockAbilityMelee::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_AI_BlockAbilityMelee::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	//Security measure - remove tags 
	if (!Melee->HasMatchingGameplayTag(LaunchTag))
	{
		Melee->RemoveGameplayTag(BlockDamageTag);
	}
	Melee->RemoveGameplayTag(CanBeStunnedTag);
	Melee->GetCharacterMovement()->RotationRate.Yaw = RotationRate;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_AI_BlockAbilityMelee::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_AI_BlockAbilityMelee::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//Targets Player/Clone/Decoy
void USATORI_AI_BlockAbilityMelee::GetTarget()
{
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetAvatarActorFromActorInfo());

	if (IsValid(Blackboard))
	{
		FName Clone = "Clone";
		FName Decoy = "Señuelo";
		FName Player = "Target";

		AActor* Target = nullptr;
		Target = Cast<AActor>(Blackboard->GetValueAsObject(Clone));
		//Check clone
		if (IsValid(Target))
		{
			Enemy = Target;
		}
		//If clone fails then checks for Decoy, if Decoy fails checks Player
		else
		{
			Target = Cast<AActor>(Blackboard->GetValueAsObject(Decoy));
			//Check Decoy
			if (IsValid(Target))
			{
				Enemy = Target;
			}
			else
			{
				Target = Cast<AActor>(Blackboard->GetValueAsObject(Player));
				//Check Player
				if (IsValid(Target))
				{
					Enemy = Target;
				}
			}
		}
	}
}

void USATORI_AI_BlockAbilityMelee::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//Starts blocking -  timer for ending
	if (EventTag == TagSpawnAbility)
	{
		bBlocking = true;
		Melee->GetCharacterMovement()->RotationRate.Yaw = Melee->GetCharacterMovement()->RotationRate.Yaw / RotationDifference;

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USATORI_AI_BlockAbilityMelee::RemoveGameplayEffect, TimeToFinish, false);
	}
}

void USATORI_AI_BlockAbilityMelee::RemoveGameplayEffect()
{
	//Removes GE
	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(BlockingTag);
	USATORI_BlueprintLibrary::RemoveGameplayEffect(Melee, GameplayTagContainer);
}

void USATORI_AI_BlockAbilityMelee::Tick(float DeltaTime)
{

	if (!IsValid(Melee))
	{
		return;
	}

	//If GE was removed ends blocking
	if (!Melee->HasMatchingGameplayTag(BlockingTag) && bBlocking)
	{
		EndBlockingAnimation();
	}

	if (bBlocking)
	{
		BlockDamage();
	}

	if (Melee->HasMatchingGameplayTag(HitTag) && bBlocking)
	{
		FGameplayTagContainer GameplayTagContainer;
		GameplayTagContainer.AddTag(HitTag);
		USATORI_BlueprintLibrary::RemoveGameplayEffect(Melee, GameplayTagContainer);

		if (USkeletalMeshComponent* Mesh = Melee->GetMesh())
		{
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				AnimInstance->Montage_JumpToSection(FName("BlockDamage"), AnimInstance->GetCurrentActiveMontage());
			}
		}
	}

	if (!Enemy)
	{
		GetTarget();
		UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetAvatarActorFromActorInfo());
		FName Player = "Target";
		AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(Player));
		if (Target == Enemy)
		{
			//TO DO: Stop Timer
			RemoveGameplayEffect();
		}
	}
}

void USATORI_AI_BlockAbilityMelee::EndBlockingAnimation()
{
	bBlocking = false;
	Melee->RemoveGameplayTag(BlockDamageTag);

	if (USkeletalMeshComponent* Mesh = Melee->GetMesh())
	{
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			AnimInstance->Montage_JumpToSection(FName("BlockEnd"), AnimInstance->GetCurrentActiveMontage());
		}
	}
}

//If Player and actor are facing each other - Blocking Damage
//If player is facing the back of the actor - No Blocking Damage
void USATORI_AI_BlockAbilityMelee::BlockDamage()
{
	FRotator Rotation = GetAvatarActorFromActorInfo()->GetActorRotation();

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FRotator PlayerRotation = PlayerCharacter->GetActorRotation();

	float Direction = Rotation.Yaw - PlayerRotation.Yaw;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Yaw: %f"), Direction));

	if ((Direction > 100 || Direction < -100))
	{
		if (!Melee->HasMatchingGameplayTag(BlockDamageTag))
		{
			Melee->AddGameplayTag(BlockDamageTag);
		}
		Melee->RemoveGameplayTag(CanBeStunnedTag);
	}
	else
	{
		if (!Melee->HasMatchingGameplayTag(CanBeStunnedTag))
		{
			Melee->AddGameplayTag(CanBeStunnedTag);
		}
		Melee->RemoveGameplayTag(BlockDamageTag);
	}
}

bool USATORI_AI_BlockAbilityMelee::IsTickable() const
{
	return IsActive();
}

bool USATORI_AI_BlockAbilityMelee::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_AI_BlockAbilityMelee::GetStatId() const
{
	return UObject::GetStatID();
}