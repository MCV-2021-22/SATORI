// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "SATORIGameMode.generated.h"

class ASATORI_WinPortalActor;

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
	UFUNCTION(BlueprintCallable)
	TArray < AActor* >& GetEnemyActorsRef() { return EnemyActors; };

	UFUNCTION(BlueprintCallable)
	void AddEnemyActor(AActor* Enemy);

	UFUNCTION(BlueprintCallable)
	void RemoveEnemyActor(AActor* Enemy);

	UFUNCTION(BlueprintCallable)
	bool IsEmptyEnemyActors();

	UFUNCTION(BlueprintCallable)
	int NumOfEnemyActors();

	UFUNCTION()
	void AddPortalActor(ASATORI_Portal* Portal);

	UFUNCTION()
	void AddWinPortalActor(ASATORI_WinPortalActor* Portal);

	UFUNCTION()
	int NumOfPortalActors();

	UFUNCTION()
	void StopAll_AI_Logic();
private:

	UPROPERTY()
	TArray < AActor* > EnemyActors;

	UPROPERTY()
	TArray < ASATORI_Portal* > PortalActors;

	UPROPERTY()
	ASATORI_WinPortalActor* WinPortalActors;
};



