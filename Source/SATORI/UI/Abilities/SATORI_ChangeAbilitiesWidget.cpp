// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Abilities/SATORI_ChangeAbilitiesWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

bool USATORI_ChangeAbilitiesWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_ChangeAbilitiesWidget::ChangeAbilityIcons(UTexture2D* NewIcons)
{
	if (AbilityIcons)
	{
		AbilityIcons->SetBrushFromTexture(NewIcons);
	}
}