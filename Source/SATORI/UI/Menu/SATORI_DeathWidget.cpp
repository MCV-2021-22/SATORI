// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SATORI_DeathWidget.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

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

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &USATORI_DeathWidget::OnQuitGameClicked);
	}
}

void USATORI_DeathWidget::NativeDestruct()
{

}

void USATORI_DeathWidget::OnDeathButtonClicked()
{
	USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstanceRef)
	{
		GameInstanceRef->SetPlayerStart(true);
		UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonExampleMap"), false);
	}
}

void USATORI_DeathWidget::OnQuitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void USATORI_DeathWidget::OnRetryClicked()
{

}
