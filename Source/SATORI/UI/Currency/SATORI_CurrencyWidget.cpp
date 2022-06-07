// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Currency/SATORI_CurrencyWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool USATORI_CurrencyWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_CurrencyWidget::SetCurrencyText(FText value)
{
	if (PandaCoinText)
	{
		PandaCoinText->SetText(value);
	}
}