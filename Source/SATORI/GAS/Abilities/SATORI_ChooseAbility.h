// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
<<<<<<<< HEAD:Source/SATORI/GAS/Abilities/SATORI_TimeSlowAbility.h
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_TimeSlowAbility.generated.h"
========
#include "Abilities/GameplayAbility.h"
#include "SATORI_ChooseAbility.generated.h"
>>>>>>>> Alejandro:Source/SATORI/GAS/Abilities/SATORI_ChooseAbility.h

/**
 * 
 */
UCLASS()
<<<<<<<< HEAD:Source/SATORI/GAS/Abilities/SATORI_TimeSlowAbility.h
class SATORI_API USATORI_TimeSlowAbility : public USATORI_GameplayAbility
========
class SATORI_API USATORI_ChooseAbility : public UGameplayAbility
>>>>>>>> Alejandro:Source/SATORI/GAS/Abilities/SATORI_ChooseAbility.h
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
