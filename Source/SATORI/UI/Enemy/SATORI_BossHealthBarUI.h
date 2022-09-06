// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_BossHealthBarUI.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SATORI_API USATORI_BossHealthBarUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Fujin_HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Fujin_NameText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Raijin_HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Raijin_NameText;

	UFUNCTION(BlueprintCallable)
	void SetHealthBarPercentage(float Fujin_HealthValue, float Raijin_HealthValue);

	UFUNCTION(BlueprintCallable)
	void SetHealthTextBlock(FText Fujin_HealthName, FText Raijin_HealthName);

private:
	class ASATORI_Fujin* Fujin = nullptr;
	class ASATORI_Raijin* Raijin = nullptr;
};
