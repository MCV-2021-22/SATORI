// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "Actors/Misile/SATORI_MisileActor.h"
#include "SATORI_MisileAbility.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_MisileAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_MisileAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* AnimMontage;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ASATORI_MisileActor> MisileActor;

	void OnTimerExpiredFinish();

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
