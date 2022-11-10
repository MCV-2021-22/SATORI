// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/SATORI_BossHealthBarUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"

bool USATORI_BossHealthBarUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	AActor* FujinActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASATORI_Fujin::StaticClass());

	if (FujinActor)
	{
		Fujin = Cast<ASATORI_Fujin>(FujinActor);
	}

	AActor* RaijinActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASATORI_Raijin::StaticClass());

	if (FujinActor)
	{
		Raijin = Cast<ASATORI_Raijin>(RaijinActor);
	}

	if (Fujin && Raijin)
	{
		Fujin_MaxHealth = Fujin->GetMaxHealth();
		Raijin_MaxHealth = Raijin->GetMaxHealth();
	}

	return true;
}

void USATORI_BossHealthBarUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Fujin && Raijin)
	{
		Raijin_HealthBar->SetPercent(Raijin->GetHealth() / Raijin->GetMaxHealth());
		Fujin_HealthBar->SetPercent(Fujin->GetHealth() / Fujin->GetMaxHealth());

		// Notify Event
		Fujin->BossHealthChanges.AddDynamic(this, &USATORI_BossHealthBarUI::SetHealthBarPercentage);
		Raijin->BossHealthChanges.AddDynamic(this, &USATORI_BossHealthBarUI::SetHealthBarPercentage);
	}

	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_ComboSystemComponent* ComboComponent = Character->GetComboSystemComponent();
		if (ComboComponent)
		{
			ComboComponent->BossHealthChanges.AddDynamic(this, &USATORI_BossHealthBarUI::SetHealthBarPercentage);
		}
	}
}

void USATORI_BossHealthBarUI::NativeDestruct()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_ComboSystemComponent* ComboComponent = Character->GetComboSystemComponent();
		if (ComboComponent)
		{
			ComboComponent->BossHealthChanges.RemoveAll(this);
		}
	}
	Super::NativeDestruct();
}

void USATORI_BossHealthBarUI::SetHealthBarPercentage(bool isBossFight)
{
	if (isBossFight)
	{
		float Current_FujinHealth = Fujin->GetHealth();
		if (Current_FujinHealth >= 0)
		{
			float Fujin_Value = Current_FujinHealth / Fujin_MaxHealth;
			if (Fujin_HealthBar)
			{
				Fujin_HealthBar->SetPercent(Fujin_Value);
			}
		}
		
		float Current_RaijinHealth = Raijin->GetHealth();
		if (Current_RaijinHealth >= 0)
		{
			float Raijin_Value = Current_RaijinHealth / Raijin_MaxHealth;
			if (Raijin_HealthBar)
			{
				Raijin_HealthBar->SetPercent(Raijin_Value);
			}
		}
	}
}

void USATORI_BossHealthBarUI::SetHealthTextBlock(FText Fujin_HealthName, FText Raijin_HealthName)
{
	// TODO
}