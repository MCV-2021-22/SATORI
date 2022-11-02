// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Mask/SATORI_AbilityMask.h"
#include "SATORI_MaksInteractUI.generated.h"

class UButton;
class USATORI_MaskVendorPopUp;

UCLASS()
class SATORI_API USATORI_MaksInteractUI : public UUserWidget
{
	GENERATED_BODY()
public:
	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UButton* AkaButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AoButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MidoriButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	USATORI_MaskVendorPopUp* AkaPopUp = nullptr;

	UPROPERTY(meta = (BindWidget))
	USATORI_MaskVendorPopUp* AoPopUp = nullptr;

	UPROPERTY(meta = (BindWidget))
	USATORI_MaskVendorPopUp* MidoriPopUp = nullptr;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void AkaMaskButtonOnClicked();

	UFUNCTION()
	void AoMaskButtonOnClicked();

	UFUNCTION()
	void MidoriMaskButtonOnClicked();

	UFUNCTION()
	void OnResumeClicked();

	bool isClicked = false;
};
