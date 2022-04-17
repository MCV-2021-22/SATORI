// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_HealthWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SATORI_API USATORI_HealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

public:
	void SetHealthBarPercentage(float value);
	void SetHealthTextBlock(FText HealthTextValue);
};
