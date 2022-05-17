// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SATORI_DashAbility.generated.h"

/**
 * 
 */

class UCharacterMovementComponent;

UCLASS()
class SATORI_API USATORI_DashAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_DashAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* AnimMontage;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	void OnTimerExpiredFinish();

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float TimeToFinish;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float CastDelay;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float DashDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float DashSpeed;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	
};
