// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SATORI_AttributeSet.generated.h"

// Uses macros from AttributeSet.h
// It will automatically generate getter and setter functions for your Attributes.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/*
* The AttributeSet defines, holds, and manages changes to Attributes
* BaseValue and CurrentValue
* - The "BaseValue" is the permanent value of the Attribute
* whereas the "CurrentValue" is the BaseValue plus temporary modifications from GameplayEffects
*/
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

protected:
	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, 
		const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

public:
	// Health Attribute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, MaxHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, Mana);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, MaxMana);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, MoveSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData MoveSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, MoveSpeedMultiplier);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, Defense);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, Attack);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SATORI|Attributes")
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(USATORI_AttributeSet, Gold);
};
