// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SATORI_MainUI.h"
#include "Health/SATORI_HealthWidget.h"
#include "Mana/SATORI_ManaWidget.h"
#include "Currency/SATORI_CurrencyWidget.h"
#include "Abilities/SATORI_ChangeAbilitiesWidget.h"
#include "Engine/Texture2D.h"

bool USATORI_MainUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_MainUI::SetHealthBarPercentage(float value)
{
	PlayerHealth->SetHealthBarPercentage(value);
}

void USATORI_MainUI::SetHealthTextBlock(float Health, float MaxHealth)
{
	uint32 HealthInterger = FMath::RoundHalfFromZero(Health);
	uint32 MaxHealthInterger = FMath::RoundHalfFromZero(MaxHealth);
	PlayerHealth->SetHealthTextBlock(FText::FromString(FString::Printf(TEXT("%i / %i"), HealthInterger, MaxHealthInterger)));
}

void USATORI_MainUI::SetManaBarPercentage(float value)
{
	PlayerMana->SetManaBarPercentage(value);
}

void USATORI_MainUI::SetManaTextBlock(float Mana, float MaxMana)
{
	uint32 ManaInterger = FMath::RoundHalfFromZero(Mana);
	uint32 MaxManaInterger = FMath::RoundHalfFromZero(MaxMana);
	PlayerMana->SetManaTextBlock(FText::FromString(FString::Printf(TEXT("%i / %i"), ManaInterger, MaxManaInterger)));
}

void USATORI_MainUI::SetCurrencyText(float currency)
{
	uint32 CurrencyInterger = FMath::RoundHalfFromZero(currency);
	PlayerCurrency->SetCurrencyText(FText::FromString(FString::Printf(TEXT("%i"), CurrencyInterger)));
}
