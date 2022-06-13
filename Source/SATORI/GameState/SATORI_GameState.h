// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SATORI_GameState.generated.h"

class ASATORI_Portal;

UCLASS()
class SATORI_API ASATORI_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASATORI_GameState();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	TArray<ASATORI_Portal*> InstancePortals;
};
