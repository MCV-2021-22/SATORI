// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "Actors/SATORI_PushSphere.h"
#include "SATORI_PushAbility.generated.h"

/**
 * 
 */

UCLASS()
class SATORI_API USATORI_PushAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_PushAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* PushAnimMontage;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) 
		override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ASATORI_PushSphere> PushSphereClass;


protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CastDelay;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "5.0", UIMax = "85"))
	float AngleRange;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SphereRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	
};
