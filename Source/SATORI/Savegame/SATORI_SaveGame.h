// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SATORI_ActorSaveData.h"
#include "SATORI_SaveGame.generated.h"

// https://www.tomlooman.com/unreal-engine-cpp-save-system/

USTRUCT(BlueprintType)
struct FSATORI_PlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName EquippedeWeapon;
};

UCLASS()
class SATORI_API USATORI_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FSATORI_PlayerSaveData PlayerSaveData;

	UPROPERTY(BlueprintReadWrite)
	FSATORI_ActorSaveData LevelActorSaveData;
};
