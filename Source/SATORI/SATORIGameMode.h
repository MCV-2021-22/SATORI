// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SATORIGameMode.generated.h"

UCLASS(minimalapi)
class ASATORIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASATORIGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> LevelName = { FName("LevelFloorType1"), FName("LevelFloorType1_2"), FName("LevelFloorType1_3") };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int NumEnemies = 0;

	UFUNCTION(BlueprintCallable)
		void OpenLevel();

	//virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};



