// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/SATORI_ChooseAbilitiesDatas.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "SATORI_ChangeAbilitiesWidget.generated.h"

class UImage;
class UTexture2D;
class UTextBlock;
class UBorder;
class UProgressBar;
struct FSATORI_AbilitiesDatas;
struct FSATORI_AbilitiesIconsCooldownDatas;
struct FSATORI_CooldownDatas;

struct FWidgetCooldownTimerInfo
{
	FTimerHandle WaitHandle;
	float TimeRemaining;
	FTimerDelegate CooldownDelegate;
	bool IsInCountDownState = false;
};

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

	UFUNCTION(BlueprintCallable)
	void HabilityCooldownDatas(FSATORI_CooldownDatas Datas);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnData(const FSATORI_AbilitiesDatas& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AllOnData(FSATORI_AbilitiesIconsDatas Data);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_CooldownDatas(FSATORI_CooldownDatas CooldownDatas);

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

	// Widget 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* FirstAbilityIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* SecondAbilityIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* LastAbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* Cooldown_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* Cooldown_2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* Cooldown_3;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HabilityCDTimeText_1;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HabilityCDTimeText_2;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HabilityCDTimeText_3;

private:
	UFUNCTION()
	void CooldownTimerCounter(FSATORI_CooldownDatas CooldownData);
	UFUNCTION()
	void HabilityCooldownCounter(FSATORI_CooldownDatas CooldownData);

	/*FTimerHandle FirstIconWaitHandle;
	FTimerHandle SecondIconWaitHandle;
	FTimerHandle ThirstIconWaitHandle;*/

	TArray<FWidgetCooldownTimerInfo> WidgetCD_Infos;

	UFUNCTION()
	void CheckCooldownRemainForUI(int HabilityId);
};
