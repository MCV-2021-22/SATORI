// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_TimeStopAbility.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_TimeStop : public USATORI_GameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	FTimerDelegate TimerDelegate;

	FTimerDelegate TimerD;

	FTimerHandle TimerHandle;

	FTimerHandle TimerH;

protected:
	UPROPERTY(EditAnywhere)
	float AbilityTime = 5.0f;

private:
	void OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	void StartTimeStop(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	void SetActorVelocity(const FGameplayAbilityActorInfo* ActorInfo);
};
