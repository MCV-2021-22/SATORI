//

#include "GAS/Abilities/SATORI_DashAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "SATORI/Character/SATORI_PlayerController.h"

USATORI_DashAbility::USATORI_DashAbility() 
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CallTrackerRegistry = CallTracker;
}

void USATORI_DashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ASATORICharacter* Character = GetOwningSatoriCharacter();
	

	if(Character)
	{
		ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
		Character->DisableInput(Controller);

	}

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

	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Tag is not valid ... "), *GetName());
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DashAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_DashAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData) 
{
	CallTracker = CallTrackerRegistry;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData) 
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	Character->GetCharacterMovement()->StopMovementImmediately();
	CallTracker = CallTrackerRegistry;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) 
{
	if (EventTag == TagEndAbility)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		Character->GetCharacterMovement()->StopMovementImmediately();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == TagSpawnAbility)
	{

		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
			
		const FVector Dash = Character->GetCharacterMovement()->GetLastInputVector();

		FTimerDelegate TimerDelegateDash = FTimerDelegate::CreateUObject(this, &USATORI_DashAbility::Dashing);
		GetWorld()->GetTimerManager().SetTimer(TimerHandleDash, TimerDelegateDash, DashSpeed, true);

	}
}

void USATORI_DashAbility::Dashing() 
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	Character->AddActorLocalOffset(Direction * DashDistance);

	CallTracker--;
	if (CallTracker == 0) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleDash);
		CallTracker = CallTrackerRegistry;
	}
}

void USATORI_DashAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	ASATORICharacter* Character = GetOwningSatoriCharacter();
	if (Character)
	{
		ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
		if (Controller)
			Character->EnableInput(Controller);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}