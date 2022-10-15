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
class USATORI_BossHealthBarUI;
class UTexture2D;
class UImage;

/*
* Player Main UI, contain health bar, coins etc
*/
UCLASS()
class SATORI_API USATORI_MainUI : public UUserWidget
{
	GENERATED_BODY()
public:
	bool Initialize() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

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

	// Boss UI
	UPROPERTY(meta = (BindWidget))
	USATORI_BossHealthBarUI* BossHealthBarUI;

	// Player Icon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* PlayerPortrair;

	// Set Player Mask Icon
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetPlayerMaskIcon(UTexture2D* PortrailImg);
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

	// General Vendor function
	void SetBossUIVisibility(bool BossUI_Visibility);
};
