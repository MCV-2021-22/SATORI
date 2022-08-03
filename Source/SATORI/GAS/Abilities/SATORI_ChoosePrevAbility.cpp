// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_ChoosePrevAbility.h"
#include "SATORICharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"

/*
 * Take Player AbilityToChoose and EnabledAbilityClasses array length and increase AbilityToChoose
 */
void USATORI_ChoosePrevAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ASATORICharacter* Player = Cast<ASATORICharacter>(ActorInfo->AvatarActor);

	if (Player)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Player->GetPlayerAbilityComponent();
		if (CurrentAbilityComponent)
		{
			CurrentAbilityComponent->SetPrevAbility();
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

}

void USATORI_ChoosePrevAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}