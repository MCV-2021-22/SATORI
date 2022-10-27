// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/SATORI_DamageExecutionCalc.h"
#include "SATORICharacter.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "GAS/SATORI_AbilitySystemComponent.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct SATORIDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)

	SATORIDamageStatics()
	{
		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(USATORI_AttributeSet, Damage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USATORI_AttributeSet, Attack, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USATORI_AttributeSet, Defense, Target, false);
	}
};

static const SATORIDamageStatics& DamageStatics()
{
	static SATORIDamageStatics DStatics;
	return DStatics;
}

USATORI_DamageExecutionCalc::USATORI_DamageExecutionCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
}

void USATORI_DamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Ability System Component
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	// Actors
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Data that is used in aggregator evaluation that is passed from the caller/game code
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// There are a few ways to send data to an ExecutionCalculation in addition to capturing Attributes
	// 1 -SetByCaller
	// Sending Data to Execution Calculations

	float Defense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParameters, Defense);
	Defense = FMath::Max<float>(Defense, 0.0f);

	float AttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluationParameters, AttackPower);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists

	// TODO Use Tag with DataAsset
	FName DamageTag;

	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here

	float MitigatedDamage = (UnmitigatedDamage) * (1 / (1 + Defense));

	if (MitigatedDamage > 0.0f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}