// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "AI/Components/Raijin/SATORI_RaijinDefensePush.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "SATORI_DefensePush.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API USATORI_DefensePush : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_DefensePush();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagEndAbility;

	FTimerDelegate TimerDelegate;

	FTimerHandle TimerHandle;

	// Particle
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* Area_Particle = nullptr;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_RaijinDefensePush> ProjectileClass;

protected:
	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

private:

	const bool bStopWhenAbilityEnds = true;
};
