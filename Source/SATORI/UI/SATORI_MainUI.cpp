// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SATORI_MainUI.h"
#include "Health/SATORI_HealthWidget.h"

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

void USATORI_MainUI::SetHealthTextBlock(float health, float MaxHealth)
{
	uint32 HealthInterger = FMath::RoundHalfFromZero(health);
	uint32 MaxHealthInterger = FMath::RoundHalfFromZero(MaxHealth);
	PlayerHealth->SetHealthTextBlock(FText::FromString(FString::Printf(TEXT("%i / %i"), HealthInterger, MaxHealthInterger)));
}