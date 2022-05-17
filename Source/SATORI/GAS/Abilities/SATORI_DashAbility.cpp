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
	TimeToFinish = 0.5f;
	CastDelay = 0.1f;
	DashDistance = 100.0f;
	DashSpeed = 50.0f;

}

void USATORI_DashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	const FVector Dash = Character->GetCharacterMovement()->GetLastInputVector();
	if (Character->GetCharacterMovement()->Velocity != FVector::ZeroVector)
	{
		Character->LaunchCharacter(Dash * DashDistance * DashSpeed, true, true);
	}


	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_DashAbility::OnTimerExpiredFinish);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeToFinish, false);

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