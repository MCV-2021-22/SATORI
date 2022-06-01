// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_ChangeAbilitiesWidget.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class SATORI_API USATORI_ChangeAbilitiesWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UImage* AbilityIcons;

	void ChangeAbilityIcons(UTexture2D* NewIcons);
};
