// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "SATORI_LevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_LevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	void LoadPlayerStats();

};
