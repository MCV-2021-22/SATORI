// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Abilities/SATORI_ChangeAbilitiesWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"

bool USATORI_ChangeAbilitiesWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_ChangeAbilitiesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Character->GetPlayerAbilityComponent();
		if (CurrentAbilityComponent)
		{
			CurrentAbilityComponent->AllAbilityIconChange.AddDynamic(this, &USATORI_ChangeAbilitiesWidget::BP_AllOnData);
		}
	}
}

void USATORI_ChangeAbilitiesWidget::NativeDestruct()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Character->GetPlayerAbilityComponent();
		if (CurrentAbilityComponent)
		{
			CurrentAbilityComponent->AbilityIconChange.RemoveAll(this); // , & USATORI_ChangeAbilitiesWidget::BP_OnData);
		}
	}

	Super::NativeDestruct();
}

void USATORI_ChangeAbilitiesWidget::ChangeBordersIcons(FSATORI_AbilitiesBordesChecker Data)
{
	if (Data.IsFirstIconAvaiable && Data.IsSecondIconAvaiable && 
		Data.IsThirstIconAvaiable)
	{
		FirstAbilityBorder->SetBrushFromTexture(nullptr);
		SecondAbilityBorder->SetBrushFromTexture(nullptr);
		ThirstAbilityBorder->SetBrushFromTexture(nullptr);
	}
	else if (Data.IsFirstIconAvaiable && BorderTexture)
	{
		FirstAbilityBorder->SetBrushFromTexture(BorderTexture);
		SecondAbilityBorder->SetBrushFromTexture(nullptr);
		ThirstAbilityBorder->SetBrushFromTexture(nullptr);
	}
	else if (Data.IsSecondIconAvaiable && BorderTexture)
	{
		FirstAbilityBorder->SetBrushFromTexture(nullptr);
		SecondAbilityBorder->SetBrushFromTexture(BorderTexture);
		ThirstAbilityBorder->SetBrushFromTexture(nullptr);
	}
	else if (Data.IsThirstIconAvaiable && BorderTexture)
	{
		FirstAbilityBorder->SetBrushFromTexture(nullptr);
		SecondAbilityBorder->SetBrushFromTexture(nullptr);
		ThirstAbilityBorder->SetBrushFromTexture(BorderTexture);
	}
}
