// Copyright Epic Games, Inc. All Rights Reserved.

#include "SATORIGameMode.h"
#include "SATORICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Savegame/SATORI_SaveGame.h"
#include "Savegame/SATORI_ActorSaveData.h"

ASATORIGameMode::ASATORIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SATORI/Character/SATORI_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("/Game/SATORI/Character/SATORI_Character"));
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

TArray<AActor*> ASATORIGameMode::GetEnemyActors()
{
	return EnemyActors;
}

void ASATORIGameMode::AddEnemyActor(AActor* Enemy)
{
	EnemyActors.Add(Enemy);
}

void ASATORIGameMode::RemoveEnemyActor(AActor* Enemy)
{
	EnemyActors.Remove(Enemy);
}

bool ASATORIGameMode::IsEmptyEnemyActors()
{
	return EnemyActors.Num() == 0 ? true : false;
}
