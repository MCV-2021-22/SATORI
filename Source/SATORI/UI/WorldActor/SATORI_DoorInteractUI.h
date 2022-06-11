// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_DoorInteractUI.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SATORI_API USATORI_DoorInteractUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* PassiveImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractText;
};
