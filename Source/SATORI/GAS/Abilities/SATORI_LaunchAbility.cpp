// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_LaunchAbility.h"
#include "SATORICharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "GAS/Tasks/SATORI_AbilityTask_StartAbilityAndWait.h"

void USATORI_LaunchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ASATORICharacter* Player = Cast<ASATORICharacter>(ActorInfo->AvatarActor);

	if (Player)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Player->PlayerGameplayAbilityComponent;
		if (CurrentAbilityComponent)
		{
			TSubclassOf<USATORI_GameplayAbility> NextAbility = CurrentAbilityComponent->GetCurrentSatoriAbility();
			if (NextAbility)
			{
				UAbilityTask_StartAbilityAndWait* CurrentStartAbility = UAbilityTask_StartAbilityAndWait::StartAbilityAndWaitForEnd(this, FName(""), NextAbility);
				if (CurrentStartAbility)
				{
					CurrentStartAbility->Activate();
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void USATORI_LaunchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}