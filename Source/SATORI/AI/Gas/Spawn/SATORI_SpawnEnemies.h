// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Character/Spawned/SATORI_Spawned.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "SATORI_SpawnEnemies.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API USATORI_SpawnEnemies : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_SpawnEnemies();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ASATORI_Spawned> EnemyToSpawn;

};
