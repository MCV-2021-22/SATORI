// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_TimeStopAbility.generated.h"

class UParticleSystem;

UCLASS()
class SATORI_API USATORI_TimeStop : public USATORI_GameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
	override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, bool bWasCancelled)
	override;

public:
	USATORI_TimeStop();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particle")
	UParticleSystem* P_Particle;

	FTimerDelegate TimerDelegate;

	FTimerDelegate TimerD;

	FTimerHandle TimerHandle;

	FTimerHandle TimerH;

protected:
	UPROPERTY(EditAnywhere)
	float AbilityTime = 5.0f;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	USATORI_GameInstance* GameInstanceRef;

private:
	const bool bStopWhenAbilityEnds = true;

	void OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	void StartTimeStop();

	void SetActorVelocity(const FGameplayAbilityActorInfo* ActorInfo);
};
