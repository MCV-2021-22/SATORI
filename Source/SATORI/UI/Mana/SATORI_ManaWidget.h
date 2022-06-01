// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_ManaWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SATORI_API USATORI_ManaWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ManaText;

public:
	void SetManaBarPercentage(float value);
	void SetManaTextBlock(FText ManaTextValue);
};
