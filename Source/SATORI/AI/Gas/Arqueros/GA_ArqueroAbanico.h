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
#include "GA_ArqueroAbanico.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UGA_ArqueroAbanico : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ArqueroAbanico();


	FTimerDelegate TimerDelegate;

	FTimerHandle TimerHandle;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_ArcherProjectile> ProjectileClass;

};
