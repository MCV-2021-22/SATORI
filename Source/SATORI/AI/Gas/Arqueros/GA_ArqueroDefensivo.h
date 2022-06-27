// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "TimerManager.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectile.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GA_ArqueroDefensivo.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UGA_ArqueroDefensivo : public UGameplayAbility
{
	GENERATED_BODY()

public:

		UGA_ArqueroDefensivo();

		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
			UAnimMontage* AnimMontage;

		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
			FGameplayTag TagSpawnAbility;

		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
			FGameplayTag TagEndAbility;


		FTimerDelegate TimerDelegate;

		FTimerHandle TimerHandle;
		FTimerHandle TimerHandle2;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_ArcherProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool Defensive = false;

	UPROPERTY(EditDefaultsOnly)
		float RemainingSeconds = 2.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsClone;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float damage;

private:

	ASATORI_AICharacter* Enemy = nullptr;

	void OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	void OnEndAb(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	int iteracion = 0;


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
