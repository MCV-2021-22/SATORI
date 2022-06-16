// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "SATORI_PlayerState.generated.h"

class USATORI_AttributeSet;
class USATORI_AbilitySystemComponent;
class UAbilitySystemComponent;

UCLASS()
class SATORI_API ASATORI_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ASATORI_PlayerState();

	USATORI_AttributeSet* GetSatoriAttributeSet() const;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Getters for attributes from USATORI_AttributeSet.
	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetDefense() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetGold() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	USATORI_AttributeSet* AttributeSetBase;

	UPROPERTY()
	USATORI_AbilitySystemComponent* AbilitySystemComponent;
};
