// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_StatsComponent.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerState.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "UI/SATORI_MainUI.h"
#include "Character/SATORI_PlayerController.h"

// Sets default values for this component's properties
USATORI_StatsComponent::USATORI_StatsComponent()
{
	
}

// Called when the game starts
void USATORI_StatsComponent::BeginPlay()
{
	Super::BeginPlay();

	SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
	ASATORI_PlayerState* PlayerState = SatoriCharacter->GetPlayerState<ASATORI_PlayerState>();

	if (SatoriCharacter && PlayerState)
	{
		InitializeStatsAttributes(PlayerState);
		BindAttributeChage(SatoriCharacter);
	}
}

void USATORI_StatsComponent::InitializeStatsAttributes(ASATORI_PlayerState* PlayerState)
{
	// Initialize attributes from player state 
	PlayerAttributes = PlayerState->GetSatoriAttributeSet();
	if (PlayerAttributes.IsValid())
	{
		MaxHealth = PlayerAttributes->GetMaxHealth();
		Health = MaxHealth;
		PandaCoins = PlayerAttributes->GetGold();
		PlayerAttributes->InitHealth(Health);
		PlayerAttributes->InitGold(PandaCoins);

		MaxMana = PlayerAttributes->GetMaxMana();
		Mana = MaxMana;
		PlayerAttributes->InitMana(Mana);

		// Update Health UI 
		UpdateHealthBarPercent();
		UpdateHealthBarText();
		// Update Health UI 
		UpdateManaBarPercent();
		UpdateManaBarText();

		// Update Coins
		UpdatePandaCoinText();
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

		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(PlayerAttributes->GetManaAttribute()).AddUObject(this, &USATORI_StatsComponent::ManaChanged);

		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(PlayerAttributes->GetMaxManaAttribute()).AddUObject(this, &USATORI_StatsComponent::MaxManaChanged);
		
		PandaCoinChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(PlayerAttributes->GetGoldAttribute()).AddUObject(this, &USATORI_StatsComponent::PandaCoinChanged);
	}
}

void USATORI_StatsComponent::HealthChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;

	Health = NewValue;

	if (Health <= 0)
	{
		SatoriCharacter->CharacterDeath();

	}

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

void USATORI_StatsComponent::ManaChanged(const FOnAttributeChangeData& Data)
{	
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;
	
	Mana = NewValue;

	UpdateManaBarPercent();
	UpdateManaBarText();
}

void USATORI_StatsComponent::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;

	MaxMana = NewValue;

	UpdateManaBarPercent();
	UpdateManaBarText();
}

void USATORI_StatsComponent::PandaCoinChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;
	
	PandaCoins = NewValue;

	UpdatePandaCoinText();
}

void USATORI_StatsComponent::UpdateHealthBarPercent()
{
	// Controller
	ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
	if (PlayerController)
	{
		USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
		if (MainUI)
		{
			MainUI->SetHealthBarPercentage(Health / MaxHealth);
		}
	}
}

void USATORI_StatsComponent::UpdateHealthBarText()
{
	// Controller
	ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
	if (PlayerController)
	{
		USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
		if (MainUI)
		{
			MainUI->SetHealthTextBlock(Health, MaxHealth);
		}
	}
}

void USATORI_StatsComponent::UpdateManaBarPercent()
{
	ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
	if (PlayerController)
	{
		USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
		if (MainUI)
		{
			MainUI->SetManaBarPercentage(Mana / MaxMana);
		}
	}
}

void USATORI_StatsComponent::UpdateManaBarText()
{
	// Controller
	ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
	if (PlayerController)
	{
		USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
		if (MainUI)
		{
			MainUI->SetManaTextBlock(Mana, MaxMana);
		}
	}
}

void USATORI_StatsComponent::UpdatePandaCoinText()
{
	// Controller
	ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
	if (PlayerController)
	{
		USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
		if (MainUI)
		{
			MainUI->SetCurrencyText(PandaCoins);
		}
	}
}
