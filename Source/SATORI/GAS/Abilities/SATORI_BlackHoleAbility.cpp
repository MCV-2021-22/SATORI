// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_BlackHoleAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "TimerManager.h"
#include "Engine/Classes/Camera/CameraComponent.h"

USATORI_BlackHoleAbility::USATORI_BlackHoleAbility() {

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Ability Tags
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Ability.BlackHole"));
	AbilityTags.AddTag(Tag);
	ActivationOwnedTags.AddTag(Tag);

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	//Ability default parameters
	TimeToFinish = 1.5f;
	CastDelay = 0.2f;

}

void USATORI_BlackHoleAbility::ActivateAbility(
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

	UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>();
	FVector CameraForward = CameraComponent->GetForwardVector();
	FRotator CameraRotation = CameraComponent->GetComponentRotation();
	CameraRotation.Pitch = 0.f;

	ASATORI_BlackHoleActor* Misile = GetWorld()->SpawnActor<ASATORI_BlackHoleActor>(
		BlackHoleActor, //Actor to Spawn
		Character->GetActorLocation() + CameraForward * 100, //Spawn location
		Character->GetActorRotation()); //Spawn rotation

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_BlackHoleAbility::OnTimerExpiredFinish);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeToFinish, false);

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void USATORI_BlackHoleAbility::OnTimerExpiredFinish()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_BlackHoleAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_BlackHoleAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_BlackHoleAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

}
