// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "SATORI_StatsComponent.generated.h"

class ASATORICharacter;
class ASATORI_PlayerState;
class USATORI_AttributeSet;
class UAbilitySystemComponent;

/*
* Class represent All Player Stats, we can use for UI 
* To listen for when an Attribute changes to update the UI or other gameplay, 
* use UAbilitySystemComponent::GetGameplayAttributeValueChangeDelegate(FGameplayAttribute Attribute)
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_StatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_StatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Initialize all Health attributes for local values
	virtual void InitializeStatsAttributes(ASATORI_PlayerState* PlayerState);

	// Helper function if we change some value attributes and we capture the changes
	virtual void BindAttributeChage(ASATORICharacter* PlayerCharacter);

	// Health Functions
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

	//Mana Functions
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);

protected:
	// Attributes 
	TWeakObjectPtr<USATORI_AttributeSet> PlayerAttributes;
	// Ability Component
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Attributes
	float MaxHealth = 0.f;
	float Health = 0.f;

	float MaxMana = 0.f;
	float Mana = 0.f;

	// Delegates
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;

	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;

	// Update UI
	void UpdateHealthBarPercent();
	void UpdateHealthBarText();

	void UpdateManaBarPercent();
	void UpdateManaBarText();

	ASATORICharacter* SatoriCharacter = nullptr;
};
