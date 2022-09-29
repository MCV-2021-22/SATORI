// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_Spawner.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_Spawner : public ASATORI_AICharacter
{
	GENERATED_BODY()

public:
	ASATORI_Spawner();
	virtual void Tick(float DeltaSeconds) override;

	int GetMaxEnemies() const;
	void AddNumEnemies(int AddOrSubtract);
	int GetNumEnemies();
	float GetCoolDown() const;
	void ResetSpawnTime();
	float GetSpawnTime();

	UPROPERTY(BlueprintReadOnly)
	bool spawning = false;

protected:

	int MaxNumSpawn = 5;
	int NumEnemies = 0;
	float CoolDown = 5.f;
	float SpawnTime = 5.f;
};