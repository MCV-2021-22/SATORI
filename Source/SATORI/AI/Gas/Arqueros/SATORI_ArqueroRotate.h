// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectile.h"
#include "SATORI_ArqueroRotate.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API USATORI_ArqueroRotate : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_ArqueroRotate();

	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;




	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsClone;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float damage;


	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	void OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int iteracion = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int max_iteracion = 4;

private:

	
	const bool bStopWhenAbilityEnds = true;
};