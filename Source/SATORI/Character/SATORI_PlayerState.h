// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SATORI_PlayerState.generated.h"

class USATORI_AttributeSet;

UCLASS()
class SATORI_API ASATORI_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASATORI_PlayerState();

	USATORI_AttributeSet* GetSatoriAttributeSet() const;

	// Getters for attributes from USATORI_AttributeSet.
	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetDefense() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMoveSpeed() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	USATORI_AttributeSet* AttributeSetBase;
};
