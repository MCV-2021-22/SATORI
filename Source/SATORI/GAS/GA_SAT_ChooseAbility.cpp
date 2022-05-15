// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_SAT_ChooseAbility.h"
#include "SATORICharacter.h"
#include "AbilitySystemComponent.h"

void UGA_SAT_ChooseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TArray<FGameplayAbilitySpec> Abilities = ActorInfo->AbilitySystemComponent->GetActivatableAbilities();

	ASATORICharacter* Player = Cast<ASATORICharacter>(ActorInfo->AvatarActor);

	ActorInfo->AbilitySystemComponent->TryActivateAbility(Abilities[Player->AbilityToChoose].Handle);

	Player->AbilityToChoose++;
	if (Player->AbilityToChoose >= Abilities.Num()-1)
		Player->AbilityToChoose = 0;

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

}

void UGA_SAT_ChooseAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}