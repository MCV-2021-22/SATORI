// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/SATORI_ManaRecoverEffect.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"

USATORI_ManaRecoverEffect::USATORI_ManaRecoverEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = USATORI_AttributeSet::GetManaAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(5.0f);

	Modifiers.Add(ModifierInfo);

}