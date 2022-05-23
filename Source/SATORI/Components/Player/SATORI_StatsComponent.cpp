// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_StatsComponent.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerState.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
USATORI_StatsComponent::USATORI_StatsComponent()
{
	
}

// Called when the game starts
void USATORI_StatsComponent::BeginPlay()
{
	Super::BeginPlay();

	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(GetOwner());
	ASATORI_PlayerState* PlayerState = PlayerCharacter->GetPlayerState<ASATORI_PlayerState>();

	if (PlayerCharacter && PlayerState)
	{
		InitializeHealthAttribute(PlayerState);
		BindAttributeChage(PlayerCharacter);
	}
}

void USATORI_StatsComponent::InitializeHealthAttribute(ASATORI_PlayerState* PlayerState)
{
	// Initialize attributes from player state 
	PlayerAttributes = PlayerState->GetSatoriAttributeSet();
	if (PlayerAttributes.IsValid())
	{
		MaxHealth = PlayerAttributes->GetMaxHealth();
		Health = MaxHealth;
		PlayerAttributes->InitHealth(Health);

		// Update Health UI 
		UpdateHealthBarPercent();
		UpdateHealthBarText();
	}
}

void USATORI_StatsComponent::BindAttributeChage(ASATORICharacter* PlayerCharacter)
{
	AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (AbilitySystemComponent.IsValid())
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(PlayerAttributes->GetHealthAttribute()).AddUObject(this, &USATORI_StatsComponent::HealthChanged);

		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(PlayerAttributes->GetMaxHealthAttribute()).AddUObject(this, &USATORI_StatsComponent::MaxHealthChanged);
	}
}

void USATORI_StatsComponent::HealthChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;

	Health = NewValue;

	UpdateHealthBarPercent();
	UpdateHealthBarText();
}

void USATORI_StatsComponent::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;

	MaxHealth = NewValue;

	UpdateHealthBarPercent();
	UpdateHealthBarText();
}

void USATORI_StatsComponent::UpdateHealthBarPercent()
{
	// Controller
}

void USATORI_StatsComponent::UpdateHealthBarText()
{
	// Controller
}