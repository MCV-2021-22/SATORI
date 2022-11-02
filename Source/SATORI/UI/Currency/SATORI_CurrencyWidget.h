// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_CurrencyWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SATORI_API USATORI_CurrencyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UImage* PandaCoinTexture;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PandaCoinText;

public:
	UFUNCTION()
	void SetCurrencyText(FText value);
};
