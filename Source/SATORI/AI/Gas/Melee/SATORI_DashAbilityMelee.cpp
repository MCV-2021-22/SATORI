//

#include "AI/Gas/Melee/SATORI_DashAbilityMelee.h"
#include "AbilitySystemComponent.h"
#include "AI/Character/SATORI_AICharacter.h"

USATORI_DashAbilityMelee::USATORI_DashAbilityMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CallTrackerRegistry = CallTracker;
}

void USATORI_DashAbilityMelee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DashAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Cannot Commit Ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbilityMelee::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DashAbilityMelee::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbilityMelee::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DashAbilityMelee::OnCancelled);
	Task->ReadyForActivation();

	FTimerDelegate TimerDelegateDash = FTimerDelegate::CreateUObject(this, &USATORI_DashAbilityMelee::Dashing);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleDash, TimerDelegateDash, DashSpeed, true);
}

void USATORI_DashAbilityMelee::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	CallTracker = CallTrackerRegistry;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbilityMelee::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_AICharacter* Melee = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	Melee->GetCharacterMovement()->StopMovementImmediately();
	CallTracker = CallTrackerRegistry;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbilityMelee::Dashing()
{
	ASATORI_AICharacter* Melee = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	Melee->AddActorLocalOffset(Direction * DashDistance);

	CallTracker--;
	if (CallTracker == 0) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleDash);
		CallTracker = CallTrackerRegistry;
	}
}