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
#include "Blueprint/UserWidget.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "SATORIGameMode.h"

// Sets default values for this component's properties
USATORI_StatsComponent::USATORI_StatsComponent()
{
	
}

// Called when the game starts
void USATORI_StatsComponent::BeginPlay()
{
	Super::BeginPlay();

	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
	BindAttributeChage(SatoriCharacter);
	/*ASATORI_PlayerState* PlayerState = SatoriCharacter->GetPlayerState<ASATORI_PlayerState>();

	if (SatoriCharacter && PlayerState)
	{
		InitializeStatsAttributes(PlayerState);
		BindAttributeChage(SatoriCharacter);
	}*/
}

void USATORI_StatsComponent::InitializeStatsAttributes(ASATORI_PlayerState* PlayerState)
{
	// Initialize attributes from player state 
	PlayerAttributes = PlayerState->GetSatoriAttributeSet();
	if (PlayerAttributes.IsValid())
	{
		MaxHealth = PlayerAttributes->GetMaxHealth();
		Health = PlayerAttributes->GetHealth();
		PandaCoins = PlayerAttributes->GetGold();
		PlayerAttributes->InitHealth(Health);
		PlayerAttributes->InitGold(PandaCoins);

		MaxMana = PlayerAttributes->GetMaxMana();
		Mana = PlayerAttributes->GetMana();
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

void USATORI_StatsComponent::InitializeStatsAttributesByInstance(ASATORI_PlayerState* PlayerState, USATORI_GameInstance* GameInstance)
{
	if (GameInstance && PlayerState)
	{
		ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
		if (SatoriCharacter)
		{
			MaxHealth = GameInstance->MaxHealth;
			Health = GameInstance->Health;
			PandaCoins = GameInstance->Gold;
			SatoriCharacter->SetHealth(Health);
			SatoriCharacter->SetMaxHealth(MaxHealth);
			SatoriCharacter->SetGold(PandaCoins);

			MaxMana = GameInstance->MaxMana;
			Mana = GameInstance->Mana;
			SatoriCharacter->SetMana(Mana);
			SatoriCharacter->SetMaxMana(MaxMana);

			MoveSpeed = GameInstance->MoveSpeed;
			Defense = GameInstance->Defense;
			Attack = GameInstance->Attack;
			SatoriCharacter->SetDefense(Defense);
			SatoriCharacter->SetAttack(Attack);
			SatoriCharacter->SetMoveSpeed(MoveSpeed);
		}

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

void USATORI_StatsComponent::UpdatePlayerStat()
{
	void UpdateHealthBarPercent();
	void UpdateHealthBarText();

	void UpdateManaBarPercent();
	void UpdateManaBarText();
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
		// Stop All AI Logic
		GetWorld()->GetAuthGameMode<ASATORIGameMode>()->StopAll_AI_Logic();

		ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
		if (SatoriCharacter)
		{
			SatoriCharacter->isPlayerDead = true;
			// Broadcast
			OnPlayerDeathBroadCastForDissolver();

			SatoriCharacter->CharacterDeath();
			ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
			if (PlayerController && SatoriCharacter->GetComboSystemComponent()->isInBossFight == true)
			{
				USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
				if (MainUI)
				{
					MainUI->SetBossUIVisibility(false);
				}
			}
		}
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
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
	if (SatoriCharacter)
	{
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
}

void USATORI_StatsComponent::UpdateHealthBarText()
{
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
	if (SatoriCharacter)
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
}

void USATORI_StatsComponent::UpdateManaBarPercent()
{
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
	if (SatoriCharacter)
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
}

void USATORI_StatsComponent::UpdateManaBarText()
{
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
	if (SatoriCharacter)
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
}

void USATORI_StatsComponent::UpdatePandaCoinText()
{
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(GetOwner());
	if (SatoriCharacter)
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
}

void USATORI_StatsComponent::OnPlayerDeathBroadCastForDissolver()
{
	FOnPlayerDeathBroadCast.Broadcast(true);
}