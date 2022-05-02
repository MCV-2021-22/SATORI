// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Savegame/SATORI_SaveGameComponent.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "SaveGame/SATORI_SaveGame.h"

// Sets default values for this component's properties
USATORI_SaveGameComponent::USATORI_SaveGameComponent()
{
	bWantsInitializeComponent = true;
}

void USATORI_SaveGameComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (USATORI_GameInstance* GameInst = GetWorld()->GetGameInstance<USATORI_GameInstance>())
	{
		// TODO
		if (USATORI_SaveGame* SaveGame = GameInst->GetSATORISaveGame())
		{
			const FName LevelName = GetWorld()->GetPackage()->GetFName();
		}
	}
}

void USATORI_SaveGameComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
