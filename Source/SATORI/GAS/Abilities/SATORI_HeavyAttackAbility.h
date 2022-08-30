// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_HeavyAttackAbility.generated.h"

class UGameplayEffect;

UCLASS()
class SATORI_API USATORI_HeavyAttackAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()
	
public:
	USATORI_HeavyAttackAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* AnimMontage;

	TSubclassOf<UGameplayEffect> PowerUpGameplayEffect;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, 
		bool bWasCancelled) override;

protected:

	// Variables
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	float MaxHoldTime = 3.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	float AttackMultiplier = 0.2f;

	const bool bStopWhenAbilityEnds = true;
	// Functions
	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnTimerFinished(float FinishTime);
};
