// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/SATORI_ChooseAbilitiesDatas.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "SATORI_ChangeAbilitiesWidget.generated.h"

class UImage;
class UTexture2D;
class UBorder;
class UProgressBar;
struct FSATORI_AbilitiesDatas;

UCLASS()
class SATORI_API USATORI_ChangeAbilitiesWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void HabilityCooldownChanges(float Opacity, bool IsCooldownvisible,
		UImage* HabilityIcon, UProgressBar* CooldownBar);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnData(const FSATORI_AbilitiesDatas& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AllOnData(FSATORI_AbilitiesIconsDatas Data);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_CooldownDatas(FSATORI_AbilitiesIconsCooldownDatas CooldownDatas);

	UFUNCTION(BlueprintCallable)
	void ChangeBordersIcons(FSATORI_AbilitiesBordesChecker Data);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* BorderTexture;

	UPROPERTY(meta = (BindWidget))
	UBorder* FirstAbilityBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* SecondAbilityBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* ThirstAbilityBorder;
};
