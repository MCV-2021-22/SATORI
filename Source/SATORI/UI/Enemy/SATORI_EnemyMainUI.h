// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_EnemyMainUI.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_EnemyMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;
};
