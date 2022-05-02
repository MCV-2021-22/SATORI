// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Savegame/SATORI_SaveGame.h"

#include "SATORI_GameInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SATORI_API USATORI_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USATORI_GameInstance();

	void Init() override;
	void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	USATORI_SaveGame* GetSATORISaveGame() { return SaveGame; }

	UFUNCTION(BlueprintCallable)
	void SaveGameToDisk();

	void RegisterActorToSave();
	void UnRegisterActorToSave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerLife = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerMana = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PlayerAbility = 0;
private:
	void InitSaveGame();

	UPROPERTY()
	USATORI_SaveGame* SaveGame = nullptr;

	UPROPERTY()
	FString SaveSlotName = "MCVSaveData";

	
};
