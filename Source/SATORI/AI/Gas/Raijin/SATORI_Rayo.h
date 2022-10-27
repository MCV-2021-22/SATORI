// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Raijin/SATORI_RaijinRayo.h"
#include "SATORI_Rayo.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API USATORI_Rayo : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_Rayo();

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


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_RaijinRayo> ProjectileClass;

	void OnBucleRayos(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		float Damage = 0.001;

protected:
	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	int iteracion = 0;
	//int max_iteracion = 4;
	int max_iteracion = 5;

private:

	const bool bStopWhenAbilityEnds = true;
};
