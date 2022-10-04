// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "AI/Character/Spawned/SATORI_Spawned.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORI_SpawnEnemies.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API USATORI_SpawnEnemies : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_SpawnEnemies();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled)
		override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag SpawnTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ASATORI_Spawned> EnemyToSpawn;

	bool bIsCreateOnRunning = false;

protected:

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);


	ASATORI_Spawner* Spawner;

private:

	const bool bStopWhenAbilityEnds = true;

};
