// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_HeavyAttackAbility.generated.h"

class UGameplayEffect;
class ASATORI_Push360Actor;

UCLASS()
class SATORI_API USATORI_HeavyAttackAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()
	
public:
	USATORI_HeavyAttackAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Montage")
	UAnimMontage* AnimMontage;

	TSubclassOf<UGameplayEffect> PowerUpGameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<ASATORI_Push360Actor> Push360Actor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagSpawnAbility;

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
	float MaxHoldTime = 4.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	float AttackMultiplier = 0.15f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	float KnockBackHoldTime = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.1"), Category = "Ability|Push360")
	float TimeToEndAbility = 0.01f;

	float FinishHoldTime = 0.0f;

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
