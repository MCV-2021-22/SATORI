// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SATORI_DeathWidget.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"

bool USATORI_DeathWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}


void USATORI_DeathWidget::NativeConstruct()
{
	if (DeathButton)
	{
		DeathButton->OnClicked.AddDynamic(this, &USATORI_DeathWidget::OnDeathButtonClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &USATORI_DeathWidget::OnMainMenuClicked);
	}
}

void USATORI_DeathWidget::NativeDestruct()
{

}

void USATORI_DeathWidget::OnDeathButtonClicked()
{
	USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (GameInstanceRef && Character)
	{
		if (!Character->GetComboSystemComponent()->isInBossFight)
		{
			GameInstanceRef->SetPlayerStart(true);
			Character->ResetCharacterDatas();
			UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonExampleMap"), false);
		}
		else if (Character->GetComboSystemComponent()->isInBossFight)
		{
			GameInstanceRef->SetPlayerStart(false);
			GameInstanceRef->IsInBossFight = true;
			UGameplayStatics::OpenLevel(GetWorld(), FName("JARDIN_BOSS"), false);
		}
	}
}

void USATORI_DeathWidget::OnMainMenuClicked()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"), false);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->ResetCharacterDatas();
	}
}

void USATORI_DeathWidget::OnRetryClicked()
{

}
