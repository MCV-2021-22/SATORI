// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_DashAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "TimerManager.h"

USATORI_DashAbility::USATORI_DashAbility() {

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Ability Tags
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Dash"));
	AbilityTags.AddTag(Tag);
	ActivationOwnedTags.AddTag(Tag);

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	//Ability default parameters
	TimeToFinish = 1.0f;
	CastDelay = 0.1f;
	DashDistance = 25.0f;
	CallTracker = 50;
	CallTrackerRegistry = CallTracker;
	DashSpeed = 0.01f;

}

void USATORI_DashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	
	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (Character->GetCharacterMovement()->Velocity != FVector::ZeroVector) {

		const FVector Dash = Character->GetCharacterMovement()->GetLastInputVector();

		FTimerDelegate TimerDelegateDash = FTimerDelegate::CreateUObject(this, &USATORI_DashAbility::Dashing);
		GetWorld()->GetTimerManager().SetTimer(TimerHandleDash, TimerDelegateDash, DashSpeed, true);


		FTimerHandle TimerHandleFinish;
		FTimerDelegate TimerDelegateFinish = FTimerDelegate::CreateUObject(this, &USATORI_DashAbility::OnTimerExpiredFinish);
		GetWorld()->GetTimerManager().SetTimer(TimerHandleFinish, TimerDelegateFinish, TimeToFinish, false);
	}
	else {
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

}

void USATORI_DashAbility::Dashing() {

	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	Character->AddActorLocalOffset(Direction * DashDistance);

	CallTracker--;
	UE_LOG(LogTemp, Display, TEXT("CallTracker: %d"), CallTracker);
	if (CallTracker == 0) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleDash);
		CallTracker = CallTrackerRegistry;
	}

}

void USATORI_DashAbility::OnTimerExpiredFinish() 
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	Character->GetCharacterMovement()->StopMovementImmediately();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData) {
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData) {
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) {

}