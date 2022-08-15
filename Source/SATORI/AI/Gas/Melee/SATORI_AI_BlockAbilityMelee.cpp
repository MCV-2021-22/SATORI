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

	if (!TagSpawnAbility.IsValid()  || !BlockDamageTag.IsValid() || !BlockingTag.IsValid())
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

void USATORI_AI_BlockAbilityMelee::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		bBlocking = true;
		Melee->GetCharacterMovement()->RotationRate.Yaw = Melee->GetCharacterMovement()->RotationRate.Yaw / RotationDifference;

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USATORI_AI_BlockAbilityMelee::EndSpecial, TimeToFinish, false);
	}
}

void USATORI_AI_BlockAbilityMelee::EndSpecial()
{
	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(BlockingTag);
	USATORI_BlueprintLibrary::RemoveGameplayEffect(Melee, GameplayTagContainer);
}

void USATORI_AI_BlockAbilityMelee::Tick(float DeltaTime)
{
	if (!Melee->HasMatchingGameplayTag(BlockingTag) && bBlocking)
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

	if (bBlocking)
	{
		FRotator Rotation = GetAvatarActorFromActorInfo()->GetActorRotation();

		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		FRotator PlayerRotation = PlayerCharacter->GetActorRotation();

		float Direction = Rotation.Yaw - PlayerRotation.Yaw;

		if ((Direction > 100 || Direction < -100))
		{
			if (!Melee->HasMatchingGameplayTag(BlockDamageTag))
			{
				Melee->AddGameplayTag(BlockDamageTag);
			}
		}
		else
		{
			Melee->RemoveGameplayTag(BlockDamageTag);
		}
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