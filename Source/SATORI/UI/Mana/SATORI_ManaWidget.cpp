// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Mana/SATORI_ManaWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool USATORI_ManaWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_ManaWidget::SetManaBarPercentage(float value)
{
	if (ManaBar)
	{
		ManaBar->SetPercent(value);
	}
}

void USATORI_ManaWidget::SetManaTextBlock(FText ManaTextValue)
{
	if (ManaText)
	{
		ManaText->SetText(ManaTextValue);
	}
}

