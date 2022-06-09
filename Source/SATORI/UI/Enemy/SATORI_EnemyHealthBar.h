// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_EnemyHealthBar.generated.h"

class UProgressBar;

UCLASS()
class SATORI_API USATORI_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	void ShowHealthBar();

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
