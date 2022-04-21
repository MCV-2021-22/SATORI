// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"

USATORI_GameInstance::USATORI_GameInstance()
{

}

void USATORI_GameInstance::Init()
{
	Super::Init();
	InitSaveGame();
}

void USATORI_GameInstance::Shutdown()
{
	Super::Shutdown();
	SaveGameToDisk();
}

void USATORI_GameInstance::SaveGameToDisk()
{
	
}

void USATORI_GameInstance::InitSaveGame()
{
	SaveGame = Cast<USATORI_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveGame)
	{
		SaveGame = Cast<USATORI_SaveGame>(UGameplayStatics::CreateSaveGameObject(USATORI_SaveGame::StaticClass()));
	}
}

void USATORI_GameInstance::RegisterActorToSave()
{

}

void USATORI_GameInstance::UnRegisterActorToSave()
{

}