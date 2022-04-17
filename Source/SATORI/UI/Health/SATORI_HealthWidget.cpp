// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Health/SATORI_HealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool USATORI_HealthWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_HealthWidget::SetHealthBarPercentage(float value)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(value);
	}
}

void USATORI_HealthWidget::SetHealthTextBlock(FText HealthTextValue)
{
	if (HealthText)
	{
		HealthText->SetText(HealthTextValue);
	}
}

