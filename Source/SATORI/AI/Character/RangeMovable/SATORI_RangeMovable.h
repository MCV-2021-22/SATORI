// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_RangeMovable.generated.h"


class USATORI_ArcherProjectileSpawnerComponent;
/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_RangeMovable : public ASATORI_AICharacter
{
	GENERATED_BODY()
	
public:
	
	ASATORI_RangeMovable();
	virtual void Tick(float DeltaTime) override;

	bool moveBackwards = false;

protected:

	float dist_attack = 800.0f;

};
