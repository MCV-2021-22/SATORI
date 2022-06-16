// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/Spawner/SATORI_Spawner.h"
#include "SATORI_Spawned.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_Spawned : public ASATORI_AICharacter
{
	GENERATED_BODY()

public:

	ASATORI_Spawned();

	void SetMySpawn(ASATORI_Spawner* SpawnToSet);

	virtual void PossessedBy(AController* NewController) override;

	float GetDistAttack() const;

	float SpawnedDie();
	

protected:

	ASATORI_Spawner* MySpawn;

	float DistAttack = 100.f;
};
