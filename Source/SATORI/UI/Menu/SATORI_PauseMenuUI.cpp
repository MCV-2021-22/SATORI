// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SATORI_PauseMenuUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"

bool USATORI_PauseMenuUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_PauseMenuUI::NativeConstruct()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &USATORI_PauseMenuUI::OnResumeClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &USATORI_PauseMenuUI::OnMainMenuClicked);
	}
}

void USATORI_PauseMenuUI::NativeDestruct()
{

}

void USATORI_PauseMenuUI::OnResumeClicked()
{
	this->RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Character)
	{
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
		}
	}
}

void USATORI_PauseMenuUI::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName, false);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->ResetCharacterDatas();
	}
}