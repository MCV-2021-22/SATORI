// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Abilities/SATORI_ChangeAbilitiesWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"

bool USATORI_ChangeAbilitiesWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_ChangeAbilitiesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Character->GetPlayerAbilityComponent();
		if (CurrentAbilityComponent)
		{
			CurrentAbilityComponent->AllAbilityIconChange.AddDynamic(this, &USATORI_ChangeAbilitiesWidget::BP_AllOnData);
			CurrentAbilityComponent->CooldownAbilityIconChange.AddDynamic(this, &USATORI_ChangeAbilitiesWidget::BP_CooldownDatas);
		}
	}
}

void USATORI_ChangeAbilitiesWidget::NativeDestruct()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Character->GetPlayerAbilityComponent();
		if (CurrentAbilityComponent)
		{
			CurrentAbilityComponent->AbilityIconChange.RemoveAll(this); // , & USATORI_ChangeAbilitiesWidget::BP_OnData);
			CurrentAbilityComponent->CooldownAbilityIconChange.RemoveAll(this); // , & USATORI_ChangeAbilitiesWidget::BP_OnData);
		}
	}

	Super::NativeDestruct();
}

void USATORI_ChangeAbilitiesWidget::ChangeBordersIcons(FSATORI_AbilitiesBordesChecker Data)
{
	if (Data.IsFirstIconAvaiable && Data.IsSecondIconAvaiable && 
		Data.IsThirstIconAvaiable)
	{
		FirstAbilityBorder->SetBrushFromTexture(nullptr);
		SecondAbilityBorder->SetBrushFromTexture(nullptr);
		ThirstAbilityBorder->SetBrushFromTexture(nullptr);
	}
	else if (Data.IsFirstIconAvaiable && BorderTexture)
	{
		FirstAbilityBorder->SetBrushFromTexture(BorderTexture);
		SecondAbilityBorder->SetBrushFromTexture(nullptr);
		ThirstAbilityBorder->SetBrushFromTexture(nullptr);
	}
	else if (Data.IsSecondIconAvaiable && BorderTexture)
	{
		FirstAbilityBorder->SetBrushFromTexture(nullptr);
		SecondAbilityBorder->SetBrushFromTexture(BorderTexture);
		ThirstAbilityBorder->SetBrushFromTexture(nullptr);
	}
	else if (Data.IsThirstIconAvaiable && BorderTexture)
	{
		FirstAbilityBorder->SetBrushFromTexture(nullptr);
		SecondAbilityBorder->SetBrushFromTexture(nullptr);
		ThirstAbilityBorder->SetBrushFromTexture(BorderTexture);
	}
}

void USATORI_ChangeAbilitiesWidget::HabilityCooldownDatas(FSATORI_AbilitiesIconsCooldownDatas Datas)
{
	// First Icon
	if (Datas.FirstIconDatas.IsCooldownAvaiable && Datas.FirstIconDatas.TimeRemained >= 0.0f)
	{
		FirstAbilityIcon->SetOpacity(0.25f);
		Cooldown_1->SetVisibility(ESlateVisibility::Visible);
		CooldownTimerCounter(Datas.FirstIconDatas);
	}
	else if(!Datas.FirstIconDatas.IsCooldownAvaiable && Datas.FirstIconDatas.TimeRemained <= 0.0f)
	{
		Cooldown_1->SetVisibility(ESlateVisibility::Hidden);
		FirstAbilityIcon->SetOpacity(1.0);
	}

	// Second Icon
	if (Datas.SecondIconDatas.IsCooldownAvaiable && Datas.SecondIconDatas.TimeRemained >= 0.0f)
	{
		SecondAbilityIcon->SetOpacity(0.25f);
		Cooldown_2->SetVisibility(ESlateVisibility::Visible);
		CooldownTimerCounter(Datas.FirstIconDatas);
	}
	else if(!Datas.SecondIconDatas.IsCooldownAvaiable && Datas.SecondIconDatas.TimeRemained <= 0.0f)
	{
		Cooldown_2->SetVisibility(ESlateVisibility::Hidden);
		SecondAbilityIcon->SetOpacity(1.0);
	}

	// Thirst Icon
	if (Datas.ThirstIconDatas.IsCooldownAvaiable && Datas.ThirstIconDatas.TimeRemained >= 0.0f)
	{
		LastAbilityIcon->SetOpacity(0.25f);
		Cooldown_3->SetVisibility(ESlateVisibility::Visible);
		CooldownTimerCounter(Datas.FirstIconDatas);
	}
	else if(!Datas.ThirstIconDatas.IsCooldownAvaiable && Datas.ThirstIconDatas.TimeRemained <= 0.0f)
	{
		Cooldown_3->SetVisibility(ESlateVisibility::Hidden);
		LastAbilityIcon->SetOpacity(1.0);
	}
}

void USATORI_ChangeAbilitiesWidget::HabilityCooldownChanges(float Opacity, bool IsCooldownvisible,
	UImage* HabilityIcon, UProgressBar* CooldownBar)
{
	if (IsCooldownvisible)
	{
		CooldownBar->SetVisibility(ESlateVisibility::Visible);
		HabilityIcon->SetOpacity(Opacity);
	}
	else
	{
		CooldownBar->SetVisibility(ESlateVisibility::Hidden);
		HabilityIcon->SetOpacity(1.0);
	}
}

void USATORI_ChangeAbilitiesWidget::CooldownTimerCounter(FSATORI_CooldownDatas CooldownData)
{
	//FTimerHandle WaitHandle;
	//GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	//	{
	//		CooldownData.TimeRemained--;
	//		if (CooldownData.TimeRemained <= 0)
	//		{
	//			CooldownData.IsCooldownAvaiable = false;
	//		}
	//	}), CooldownData.TimeRemained, true);

	FTimerHandle CooldownTimerHandle;
	FTimerDelegate CooldownDelegate;

	CooldownDelegate = FTimerDelegate::CreateUObject(this, &USATORI_ChangeAbilitiesWidget::HabilityCooldownCounter, CooldownData);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, CooldownDelegate, CooldownData.TimeRemained, false);
}

void USATORI_ChangeAbilitiesWidget::HabilityCooldownCounter(FSATORI_CooldownDatas CooldownData)
{
	CooldownData.TimeRemained--;
	if (CooldownData.TimeRemained <= 0)
	{
		CooldownData.IsCooldownAvaiable = false;
	}
}