// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_MainUI.generated.h"

class USATORI_HealthWidget;
class USATORI_ManaWidget;
class USATORI_CurrencyWidget;
class USATORI_ChangeAbilitiesWidget;
class USATORI_MaksInteractUI;
class USATORI_GeneralVendorInteractUI;
class UTexture2D;

/*
* Player Main UI, contain health bar, coins etc
*/
UCLASS()
class SATORI_API USATORI_MainUI : public UUserWidget
{
	GENERATED_BODY()
public:
	bool Initialize() override;

	// Health widge
	UPROPERTY(meta = (BindWidget))
	USATORI_HealthWidget* PlayerHealth;

	UPROPERTY(meta = (BindWidget))
	USATORI_ManaWidget* PlayerMana;
	
	// Currency
	UPROPERTY(meta = (BindWidget))
	USATORI_CurrencyWidget* PlayerCurrency;

	// Abilities
	UPROPERTY(meta = (BindWidget))
	USATORI_ChangeAbilitiesWidget* PlayerAbilitiesWidget;

	// Mask Vendor
	UPROPERTY(meta = (BindWidget))
	USATORI_MaksInteractUI* MaskVendorWidget;

	// General Vendor
	UPROPERTY(meta = (BindWidget))
	USATORI_GeneralVendorInteractUI* GeneralVendorWidget;
public:
	// Health helper functions
	void SetHealthBarPercentage(float value);
	void SetHealthTextBlock(float Health, float MaxHealth);

	// Mana helper functions
	void SetManaBarPercentage(float value);
	void SetManaTextBlock(float Mana, float MaxMana);

	// Currency function
	void SetCurrencyText(float currency);

	// Mask Vendor function
	void SetMaskVendorVisibility(bool Vendor_Visibility);

	// General Vendor function
	void SetGeneralVendorVisibility(bool Vendor_Visibility);
};
