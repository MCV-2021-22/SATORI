// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SATORI.h"
#include "SATORI_GameplayAbility.generated.h"


class UTexture2D;

UCLASS()
class SATORI_API USATORI_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	USATORI_GameplayAbility();

	// To implement passive GameplayAbilities that automatically activate and run continuously.
	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

	UFUNCTION(BlueprintCallable, Category = Ability)
	class ASATORICharacter* GetOwningSatoriCharacter() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated = false;

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	ESATORI_AbilityInputID AbilityInputID = ESATORI_AbilityInputID::None;
	// Gameplay Effect
	//virtual void ApplyEffectContainer();
	//virtual void ApplyEffectContainerSpec();

	// Ability Property
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityName;

	UFUNCTION(BlueprintCallable)
	FName GetAbilityName() const;
};
