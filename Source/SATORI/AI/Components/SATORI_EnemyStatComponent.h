// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "SATORI_EnemyStatComponent.generated.h"

class ASATORI_AICharacter;
class USATORI_AttributeSet;
class UAbilitySystemComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SATORI_API USATORI_EnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USATORI_EnemyStatComponent();

	// Helper function if we change some value attributes and we capture the changes
	virtual void BindAttributeChage(ASATORI_AICharacter* SelfCharacter);
	virtual void InitializeStatsAttributes(ASATORI_AICharacter* SelfCharacter);

	// Health Functions
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "HealthChanged"))
	void UpdateHealthBarPercent(float health, float maxHealth);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	ASATORI_AICharacter* SelfCharacter = nullptr;

	// Attributes 
	TWeakObjectPtr<USATORI_AttributeSet> SATORIAttributes;
	// Ability Component
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Attributes
	float MaxHealth = 0.f;
	float Health = 0.f;

	// Delegates
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
};
