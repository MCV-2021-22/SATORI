// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SATORI_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	USATORI_GameplayAbility();

	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;
};
