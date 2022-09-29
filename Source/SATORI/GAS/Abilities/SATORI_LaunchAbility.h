// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_LaunchAbility.generated.h"

class ASATORICharacter;
class USATORI_GameplayAbility;
class USATORI_GameplayAbilityComponent;

UCLASS()
class SATORI_API USATORI_LaunchAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void ApplyCooldownToAbilityUI(TSubclassOf<USATORI_GameplayAbility> Ability, ASATORICharacter* Player);
};
