// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SATORI_WinMenus.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"

bool USATORI_WinMenus::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_WinMenus::NativeConstruct()
{
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &USATORI_WinMenus::OnMainMenuClicked);
	}
}

void USATORI_WinMenus::NativeDestruct()
{

}

void USATORI_WinMenus::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName, false);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->ResetCharacterDatas();
	}
}