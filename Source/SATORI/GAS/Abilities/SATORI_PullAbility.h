// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "Actors/Pull/SATORI_PullActor.h"
#include "SATORI_PullAbility.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_PullAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_PullAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* AnimMontage;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	void OnTimerExpiredFinish();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ASATORI_PullActor> PullActor;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float TimeToFinish;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float CastDelay;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	
};
