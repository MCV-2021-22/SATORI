// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_GeneralVendorInteractUI.generated.h"

class UButton;

UCLASS()
class SATORI_API USATORI_GeneralVendorInteractUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UButton* BuyItemButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void BuyItem();

	UFUNCTION()
	void ClosePanel();

	bool isClicked = false;
};
