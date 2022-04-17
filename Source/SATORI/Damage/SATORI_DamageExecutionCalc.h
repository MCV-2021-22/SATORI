// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SATORI_DamageExecutionCalc.generated.h"

/**
 * GameplayEffectExecutionCalculations are the most powerful way for GameplayEffects to make changes to an ASC.
 * Like ModifierMagnitudeCalculations, these can capture Attributes and optionally snapshot them. 
 * Unlike MMCs, these can change more than one Attribute and essentially do anything else that the programmer wants. 
 * ExecutionCalculations can only be used with Instant and Periodic GameplayEffects
 * Calculating damage received based on a complex formula reading from many attributes on the Source and the Target is the most common example of an ExecCalc
 */
UCLASS()
class SATORI_API USATORI_DamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	USATORI_DamageExecutionCalc();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
};
