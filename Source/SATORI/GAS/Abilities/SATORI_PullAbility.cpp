// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_PullAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "TimerManager.h"

USATORI_PullAbility::USATORI_PullAbility() {

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Ability Tags
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Pull"));
	AbilityTags.AddTag(Tag);
	ActivationOwnedTags.AddTag(Tag);

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	//Ability default parameters
	TimeToFinish = 1.5f;
	CastDelay = 0.2f;

}

void USATORI_PullAbility::ActivateAbility(
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

	ASATORI_PullActor* Pull = GetWorld()->SpawnActor<ASATORI_PullActor>(
		PullActor, //Actor to Spawn
		Character->GetActorLocation() + Character->GetActorForwardVector() * 100, //Spawn location
		Character->GetActorRotation()); //Spawn rotation

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_PullAbility::OnTimerExpiredFinish);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeToFinish, false);

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void USATORI_PullAbility::OnTimerExpiredFinish()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_PullAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_PullAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_PullAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

}