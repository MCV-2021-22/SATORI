// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "FMODBlueprintStatics.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "SATORI_TPFront.generated.h"


class UNiagaraSystem;

UCLASS()
class SATORI_API USATORI_TPFront : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_TPFront();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagEndAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
		UFMODEvent* EventLaugh = nullptr;

	FTimerDelegate TimerDelegate;

	FTimerHandle TimerHandle;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void Teleport(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	
	// Particles
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particle")
	UNiagaraSystem* Teleport_Particle = nullptr;

protected:

	int iteracion = 0;
	int max_iteracion = 4;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
private:

	const bool bStopWhenAbilityEnds = true;
};
