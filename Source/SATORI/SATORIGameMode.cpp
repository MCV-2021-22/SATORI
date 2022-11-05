// Copyright Epic Games, Inc. All Rights Reserved.

#include "SATORIGameMode.h"
#include "SATORICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Savegame/SATORI_SaveGame.h"
#include "Savegame/SATORI_ActorSaveData.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "Actors/Portal/SATORI_WinPortalActor.h"
#include "AI/Character/SATORI_AICharacter.h"

ASATORIGameMode::ASATORIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SATORI/Character/SATORI_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("/Game/SATORI/SATORI_Character"));
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASATORIGameMode::OpenLevel()
{

	int lastIndex = LevelName.Num();

	int arrayValue = FMath::RandRange(0, lastIndex);

}

//void ASATORIGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
//{
//	// Restor level Actor state
//	USATORI_GameInstance* GameInst = GetWorld()->GetGameInstance<USATORI_GameInstance>();
//	if (GameInst)
//	{
//		if (USATORI_SaveGame* SaveGame = GameInst->GetSATORISaveGame())
//		{
//			const FName LevelName = GetWorld()->GetPackage()->GetFName();
//
//			FSATORI_ActorDeleteData* DeleteActors = SaveGame->LevelActorSaveData.DeletedActorsByLevel.Find(LevelName);
//			if (DeleteActors)
//			{
//				
//			}
//		}
//	}
//}

void ASATORIGameMode::AddEnemyActor(AActor* Enemy)
{
	EnemyActors.Add(Enemy);
}

void ASATORIGameMode::RemoveEnemyActor(AActor* Enemy)
{
	EnemyActors.Remove(Enemy);

	if (IsEmptyEnemyActors())
	{
		for (ASATORI_Portal* Portal : PortalActors)
		{
			Portal->ActivatePortal();
			Portal->SetCurrentMeshInteractability(true);
		}

		if (WinPortalActors)
		{
			WinPortalActors->ActivatePortal();
			WinPortalActors->SetCurrentMeshInteractability(true);
		}
	}
}

bool ASATORIGameMode::IsEmptyEnemyActors()
{
	return EnemyActors.Num() == 0 ? true : false;
}

int ASATORIGameMode::NumOfEnemyActors()
{
	return EnemyActors.Num();
}

void ASATORIGameMode::AddPortalActor(ASATORI_Portal* Portal)
{
	PortalActors.Add(Portal);
}

void ASATORIGameMode::AddWinPortalActor(ASATORI_WinPortalActor* Portal)
{
	WinPortalActors = Portal;
}

int ASATORIGameMode::NumOfPortalActors()
{
	return PortalActors.Num();
}

void ASATORIGameMode::StopAll_AI_Logic()
{
	for (int i = 0; i < EnemyActors.Num(); i++)
	{
		ASATORI_AICharacter* AI_Enemy = Cast<ASATORI_AICharacter>(EnemyActors[i]);
		if (AI_Enemy)
		{
			AI_Enemy->StopAILogic();
		}
	}
}