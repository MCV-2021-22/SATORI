// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SATORI_AttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SATORI_API USATORI_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	USATORI_AttributeSet();

	// Respond to changes to an Attribute's Current Value before changes happen.
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Only triggers after changes to the BaseValue of an Attribute from an instant GameplayEffect
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	// Health Attribute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, MaxHealth);
};
