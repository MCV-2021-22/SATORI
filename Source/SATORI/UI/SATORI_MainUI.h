// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_MainUI.generated.h"

class USATORI_HealthWidget;
class USATORI_ManaWidget;

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

public:
	// Health helper functions
	void SetHealthBarPercentage(float value);
	void SetHealthTextBlock(float Health, float MaxHealth);

	// Mana helper functions
	void SetManaBarPercentage(float value);
	void SetManaTextBlock(float Mana, float MaxMana);
};
