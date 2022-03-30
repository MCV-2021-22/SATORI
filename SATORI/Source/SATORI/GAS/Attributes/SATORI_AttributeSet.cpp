// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

USATORI_AttributeSet::USATORI_AttributeSet()
{

}

void USATORI_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	// Health
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealthAttribute().GetNumericValue(this));
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.001f, 10000.0f);
	}

}

void USATORI_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}