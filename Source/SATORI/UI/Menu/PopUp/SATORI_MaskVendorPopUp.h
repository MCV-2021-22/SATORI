// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_MaskVendorPopUp.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class SATORI_API USATORI_MaskVendorPopUp : public UUserWidget
{
	GENERATED_BODY()
public:
	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MultiplicationText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnWindowClosed();
};
