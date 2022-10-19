// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SATORI_MainUI.h"
#include "Health/SATORI_HealthWidget.h"
#include "Mana/SATORI_ManaWidget.h"
#include "Currency/SATORI_CurrencyWidget.h"
#include "WorldActor/SATORI_MaksInteractUI.h"
#include "WorldActor/SATORI_GeneralVendorInteractUI.h"
#include "Abilities/SATORI_ChangeAbilitiesWidget.h"
#include "Enemy/SATORI_BossHealthBarUI.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "SATORICharacter.h"
#include "Character/Mask/SATORI_AbilityMask.h"

bool USATORI_MainUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (MaskVendorWidget)
	{
		MaskVendorWidget->RemoveFromViewport();
	}

	if (GeneralVendorWidget)
	{
		GeneralVendorWidget->RemoveFromViewport();
	}

	if(IsValid(BossHealthBarUI))
		BossHealthBarUI->SetVisibility(ESlateVisibility::Hidden);
	//BossHealthBarUI->RemoveFromViewport();

	return true;
}

void USATORI_MainUI::NativeConstruct()
{
	Super::NativeConstruct();
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_AbilityMask* PlayerMask = Character->SATORIAbilityMaskComponent;
		if (PlayerMask)
		{
			PlayerMask->PortrailImageChange.AddDynamic(this, &USATORI_MainUI::BP_SetPlayerMaskIcon);
		}
	}
}

void USATORI_MainUI::NativeDestruct()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_AbilityMask* PlayerMask = Character->SATORIAbilityMaskComponent;
		if (PlayerMask)
		{
			PlayerMask->PortrailImageChange.RemoveAll(this);
		}
	}
	Super::NativeDestruct();
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

void USATORI_MainUI::SetMaskVendorVisibility(bool Vendor_Visibility)
{
	if (Vendor_Visibility)
	{
		MaskVendorWidget->AddToViewport();
	}
	else
	{
		MaskVendorWidget->RemoveFromViewport();
	}
}

void USATORI_MainUI::SetGeneralVendorVisibility(bool Vendor_Visibility)
{
	if (Vendor_Visibility)
	{
		GeneralVendorWidget->AddToViewport();
	}
	else
	{
		GeneralVendorWidget->RemoveFromViewport();
	}
}

void USATORI_MainUI::SetBossUIVisibility(bool BossUI_Visibility)
{
	if (BossUI_Visibility)
	{
		BossHealthBarUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BossHealthBarUI->SetVisibility(ESlateVisibility::Hidden);
	}
}
