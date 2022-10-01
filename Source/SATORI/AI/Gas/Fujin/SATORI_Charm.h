// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Fujin/SATORI_FujinCharm.h"
#include "SATORI_Charm.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API USATORI_Charm : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_Charm();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagEndAbility;

	FTimerDelegate TimerDelegate;

	FTimerHandle TimerHandle;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void CheckCharmDone(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_FujinCharm> Charm;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsClone;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float damage;

protected:

	int iteracion = 0;
	int max_iteracion = 4;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	ASATORI_FujinCharm* CharmDone;
private:

	const bool bStopWhenAbilityEnds = true;

	float time_alive = 4.0f;
};
