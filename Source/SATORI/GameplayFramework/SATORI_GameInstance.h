// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "Savegame/SATORI_SaveGame.h"
#include "Character/Mask/SATORI_AbilityMask.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void SetPlayerStart(bool PlayerStartChange) { PlayerStart =  PlayerStartChange; }

	void RegisterActorToSave();
	void UnRegisterActorToSave();

	void SavePortalAbilitiesData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMana = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gold = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerAbility = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	SATORIMaskType MaskType = SATORIMaskType::NONE;

	bool PlayerStart = true;

	bool TimeSlow = false;

	bool TimeStop = false;

	bool isInteractWithMaskVendor = false;

	// Abilities
	TArray<FSATORI_AbilitiesDatas> NormalAbilities;
	TArray<FSATORI_AbilitiesDatas> UpgratedAbilities;
private:
	void InitSaveGame();

	UPROPERTY()
	USATORI_SaveGame* SaveGame = nullptr;

	UPROPERTY()
	FString SaveSlotName = "MCVSaveData";

};
