// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Abilities/SATORI_ChangeAbilitiesWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool USATORI_ChangeAbilitiesWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	FWidgetCooldownTimerInfo LocalAbilityCD_Info;
	LocalAbilityCD_Info.TimeRemaining = 0.f;

	WidgetCD_Infos.Init(LocalAbilityCD_Info, 3);
	// Fill Empty CD Info Array
	//for (int i = 0; i < 2; i++)
	//{
	//	WidgetCD_Infos.Add(LocalAbilityCD_Info);
	//}

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

void USATORI_ChangeAbilitiesWidget::HabilityCooldownDatas(FSATORI_CooldownDatas Datas)
{
	// First Icon
	if (Datas.AbilityID == 0)
	{
		if (Datas.IsCooldownAvaiable && Datas.TimeRemained >= 0.0f)
		{
			FirstAbilityIcon->SetOpacity(0.50f);
			Cooldown_1->SetVisibility(ESlateVisibility::Visible);
			HabilityCDTimeText_1->SetVisibility(ESlateVisibility::Visible);

			// info
			WidgetCD_Infos[Datas.AbilityID].TimeRemaining = Datas.TimeRemained;
			CooldownTimerCounter(Datas);
		}
		else if (!Datas.IsCooldownAvaiable && Datas.TimeRemained <= 0.0f && Datas.AbilityID == 0)
		{
			CheckCooldownRemainForUI(Datas.AbilityID);
		}
	}

	// Second Icon
	else if (Datas.AbilityID == 1)
	{
		if (Datas.IsCooldownAvaiable && Datas.TimeRemained >= 0.0f)
		{
			SecondAbilityIcon->SetOpacity(0.50f);
			Cooldown_2->SetVisibility(ESlateVisibility::Visible);
			HabilityCDTimeText_2->SetVisibility(ESlateVisibility::Visible);

			// info
			WidgetCD_Infos[Datas.AbilityID].TimeRemaining = Datas.TimeRemained;
			CooldownTimerCounter(Datas);
		}
		else if (!Datas.IsCooldownAvaiable && Datas.TimeRemained <= 0.0f)
		{
			CheckCooldownRemainForUI(Datas.AbilityID);
		}
	}

	// Thirst Icon
	else if (Datas.AbilityID == 2)
	{
		if (Datas.IsCooldownAvaiable && Datas.TimeRemained >= 0.0f)
		{
			LastAbilityIcon->SetOpacity(0.50f);
			Cooldown_3->SetVisibility(ESlateVisibility::Visible);
			HabilityCDTimeText_3->SetVisibility(ESlateVisibility::Visible);

			// info
			WidgetCD_Infos[Datas.AbilityID].TimeRemaining = Datas.TimeRemained;
			CooldownTimerCounter(Datas);
		}
		else if (!Datas.IsCooldownAvaiable && Datas.TimeRemained <= 0.0f && Datas.AbilityID == 1)
		{
			CheckCooldownRemainForUI(Datas.AbilityID);
		}
	}

	// First Icon
	//if (Datas.FirstIconDatas.IsCooldownAvaiable && Datas.FirstIconDatas.TimeRemained >= 0.0f 
	//	&& !WidgetCD_Infos[Datas.FirstIconDatas.AbilityID].IsInCountDownState)
	//{
	//	FirstAbilityIcon->SetOpacity(0.25f);
	//	Cooldown_1->SetVisibility(ESlateVisibility::Visible);

	//	// info
	//	WidgetCD_Infos[Datas.FirstIconDatas.AbilityID].TimeRemaining = Datas.FirstIconDatas.TimeRemained;
	//	CooldownTimerCounter(Datas.FirstIconDatas);
	//}
	//else if(!Datas.FirstIconDatas.IsCooldownAvaiable && Datas.FirstIconDatas.TimeRemained <= 0.0f)
	//{
	//	Cooldown_1->SetVisibility(ESlateVisibility::Hidden);
	//	FirstAbilityIcon->SetOpacity(1.0);
	//	WidgetCD_Infos[Datas.FirstIconDatas.AbilityID].IsInCountDownState = false;
	//}

	//// Second Icon
	//if (Datas.SecondIconDatas.IsCooldownAvaiable && Datas.SecondIconDatas.TimeRemained >= 0.0f
	//	&& !WidgetCD_Infos[Datas.FirstIconDatas.AbilityID].IsInCountDownState)
	//{
	//	SecondAbilityIcon->SetOpacity(0.25f);
	//	Cooldown_2->SetVisibility(ESlateVisibility::Visible);

	//	// info
	//	WidgetCD_Infos[Datas.SecondIconDatas.AbilityID].TimeRemaining = Datas.SecondIconDatas.TimeRemained;
	//	CooldownTimerCounter(Datas.SecondIconDatas);
	//}
	//else if(!Datas.SecondIconDatas.IsCooldownAvaiable && Datas.SecondIconDatas.TimeRemained <= 0.0f)
	//{
	//	Cooldown_2->SetVisibility(ESlateVisibility::Hidden);
	//	SecondAbilityIcon->SetOpacity(1.0);
	//	WidgetCD_Infos[Datas.FirstIconDatas.AbilityID].IsInCountDownState = false;
	//}

	//// Thirst Icon
	//if (Datas.ThirstIconDatas.IsCooldownAvaiable && Datas.ThirstIconDatas.TimeRemained >= 0.0f
	//	&& !WidgetCD_Infos[Datas.FirstIconDatas.AbilityID].IsInCountDownState)
	//{
	//	LastAbilityIcon->SetOpacity(0.25f);
	//	Cooldown_3->SetVisibility(ESlateVisibility::Visible);

	//	// info
	//	WidgetCD_Infos[Datas.ThirstIconDatas.AbilityID].TimeRemaining = Datas.ThirstIconDatas.TimeRemained;
	//	CooldownTimerCounter(Datas.ThirstIconDatas);
	//}
	//else if(!Datas.ThirstIconDatas.IsCooldownAvaiable && Datas.ThirstIconDatas.TimeRemained <= 0.0f)
	//{
	//	Cooldown_3->SetVisibility(ESlateVisibility::Hidden);
	//	LastAbilityIcon->SetOpacity(1.0);
	//	WidgetCD_Infos[Datas.FirstIconDatas.AbilityID].IsInCountDownState = false;
	//}
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
	WidgetCD_Infos[CooldownData.AbilityID].CooldownDelegate = FTimerDelegate::CreateUObject(this,
		&USATORI_ChangeAbilitiesWidget::HabilityCooldownCounter, CooldownData);

	GetWorld()->GetTimerManager().SetTimer(WidgetCD_Infos[CooldownData.AbilityID].WaitHandle,
		WidgetCD_Infos[CooldownData.AbilityID].CooldownDelegate, 1, true, 0.0);
}

void USATORI_ChangeAbilitiesWidget::HabilityCooldownCounter(FSATORI_CooldownDatas CooldownData)
{
	WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining -= 1.0f;
	WidgetCD_Infos[CooldownData.AbilityID].IsInCountDownState = true;

	// If no more Cooldown
	if (WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining <= 0)
	{
		WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining = 0.0f;
		WidgetCD_Infos[CooldownData.AbilityID].IsInCountDownState = false;
		CheckCooldownRemainForUI(CooldownData.AbilityID);
		GetWorld()->GetTimerManager().ClearTimer(WidgetCD_Infos[CooldownData.AbilityID].WaitHandle);
	}

	/*UE_LOG(LogTemp, Warning, TEXT("Local Time : %f, Ability ID : %d"), 
		WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining, CooldownData.AbilityID);*/
	if (WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining >= 0)
	{
		if (CooldownData.AbilityID == 0)
		{
			//Cooldown_1->SetPercent(WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining / CooldownData.TimeRemained);
			FNumberFormattingOptions Opts;
			Opts.SetMaximumFractionalDigits(0);
			HabilityCDTimeText_1->SetText(FText::AsNumber(WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining, &Opts));
		}
		else if (CooldownData.AbilityID == 1)
		{
			//Cooldown_2->SetPercent(WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining / CooldownData.TimeRemained);
			FNumberFormattingOptions Opts;
			Opts.SetMaximumFractionalDigits(0);
			HabilityCDTimeText_2->SetText(FText::AsNumber(WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining, &Opts));
		}
		else if (CooldownData.AbilityID == 2)
		{
			//Cooldown_3->SetPercent(WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining / CooldownData.TimeRemained);
			FNumberFormattingOptions Opts;
			Opts.SetMaximumFractionalDigits(0);
			HabilityCDTimeText_3->SetText(FText::AsNumber(WidgetCD_Infos[CooldownData.AbilityID].TimeRemaining, &Opts));
		}
	}
}

void USATORI_ChangeAbilitiesWidget::CheckCooldownRemainForUI(int HabilityId)
{
	if (HabilityId == 0)
	{
		Cooldown_1->SetVisibility(ESlateVisibility::Hidden);
		HabilityCDTimeText_1->SetVisibility(ESlateVisibility::Hidden);
		FirstAbilityIcon->SetOpacity(1.0);
		WidgetCD_Infos[HabilityId].IsInCountDownState = false;
	}
	else if (HabilityId == 1)
	{
		Cooldown_2->SetVisibility(ESlateVisibility::Hidden);
		HabilityCDTimeText_2->SetVisibility(ESlateVisibility::Hidden);
		SecondAbilityIcon->SetOpacity(1.0);
		WidgetCD_Infos[HabilityId].IsInCountDownState = false;
	}
	else if (HabilityId == 2)
	{
		Cooldown_3->SetVisibility(ESlateVisibility::Hidden);
		HabilityCDTimeText_3->SetVisibility(ESlateVisibility::Hidden);
		LastAbilityIcon->SetOpacity(1.0);
		WidgetCD_Infos[HabilityId].IsInCountDownState = false;
	}
}