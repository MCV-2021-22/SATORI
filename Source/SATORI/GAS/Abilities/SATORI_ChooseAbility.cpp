// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_ChooseAbility.h"
#include "SATORICharacter.h"
#include "AbilitySystemComponent.h"

void USATORI_ChooseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ASATORICharacter* Player = Cast<ASATORICharacter>(ActorInfo->AvatarActor);

	Player->AbilityToChoose++;
	if (Player->AbilityToChoose >= 2)
		Player->AbilityToChoose = 0;

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

}

void USATORI_ChooseAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}