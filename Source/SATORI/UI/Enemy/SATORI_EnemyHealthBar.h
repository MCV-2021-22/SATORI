// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_EnemyHealthBar.generated.h"

class UProgressBar;
class ASATORI_CharacterBase;

UCLASS()
class SATORI_API USATORI_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float health, float maxHealth);

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY()
	ASATORI_CharacterBase* Character;
};
