// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "SATORIGameMode.generated.h"

UCLASS(minimalapi)
class ASATORIGameMode : public AGameMode
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

	//EnemyActors
	UFUNCTION()
	TArray < AActor* >& GetEnemyActorsRef() { return EnemyActors; };

	UFUNCTION()
	void AddEnemyActor(AActor* Enemy);

	UFUNCTION(BlueprintCallable)
	void RemoveEnemyActor(AActor* Enemy);

	UFUNCTION()
	bool IsEmptyEnemyActors();

	UFUNCTION()
	int NumOfEnemyActors();

	UFUNCTION()
	void AddPortalActor(ASATORI_Portal* Portal);

	UFUNCTION()
	int NumOfPortalActors();

private:

	UPROPERTY()
	TArray < AActor* > EnemyActors;

	UPROPERTY()
	TArray < ASATORI_Portal* > PortalActors;

};



