// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SATORI_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASATORI_PlayerController(const class FObjectInitializer& InitializerObject);

protected:
	virtual void OnPossess(APawn* aPawn) override;
};
