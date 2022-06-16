// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "SATORI_EnemyStatComponent.generated.h"

class ASATORI_AICharacter;
class USATORI_AttributeSet;
class UAbilitySystemComponent;
class USATORI_EnemyStatComponent;
class UActorComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FHealthChangedSignature, USATORI_EnemyStatComponent, UpdateHealthBarPercent, float, Health, float, MaxHealth);

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

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "HealthChanged"))
	FHealthChangedSignature UpdateHealthBarPercent;

	float GetHealth();
	float GetMaxHealth();
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
