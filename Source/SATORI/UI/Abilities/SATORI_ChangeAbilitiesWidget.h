// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/SATORI_ChooseAbilitiesDatas.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "SATORI_ChangeAbilitiesWidget.generated.h"

class UImage;
class UTexture2D;
struct FSATORI_AbilitiesDatas;

UCLASS()
class SATORI_API USATORI_ChangeAbilitiesWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnData(const FSATORI_AbilitiesDatas& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AllOnData(FSATORI_AbilitiesIconsDatas Data);
};
