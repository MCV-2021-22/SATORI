// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_Raijin.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_Raijin : public ASATORI_AICharacter
{
	GENERATED_BODY()

public:
	ASATORI_Raijin();

	virtual void BeginPlay() override;

	float getSoCloseDist();

	UFUNCTION(BlueprintCallable)
	FVector getSpawnPos();

	void setSpawnPos(FVector Pos);



protected:

	FVector SpawnPos;

	float SoCloseDist = 120.0f;

};
