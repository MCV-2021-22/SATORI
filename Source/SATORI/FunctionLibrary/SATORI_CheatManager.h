// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "SATORI_CheatManager.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_CheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec)
	void UnlockAllAbilities(bool Value);

	UFUNCTION(Exec)
	void SetEasyMode(bool Value);

	UFUNCTION(Exec)
	void IncreasedAttack();

	UFUNCTION(Exec)
	void ShowMainUI();

	UFUNCTION(Exec)
	void SetPlayerHealthToOne();

	UFUNCTION(Exec)
	void PlayerInstaDead();
};
