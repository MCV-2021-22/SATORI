// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "SATORI_MovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_MovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:
	USATORI_MovementComponent();

	virtual float GetMaxSpeed() const override;
};
