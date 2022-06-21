// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectile.h"
#include "GA_ArquerosMisil.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API UGA_ArquerosMisil : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_ArquerosMisil();



	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_ArcherProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool IsClone;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float damage;


};
