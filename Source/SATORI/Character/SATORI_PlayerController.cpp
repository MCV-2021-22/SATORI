// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_PlayerController.h"
#include "UI/SATORI_MainUI.h"
#include "UObject/ConstructorHelpers.h"

ASATORI_PlayerController::ASATORI_PlayerController(const class FObjectInitializer& InitializerObject)
	: Super(InitializerObject)
{
	
}

// Called when the game starts
void ASATORI_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*if (SATORIMainUI)
	{
		SATORIMainWidget = CreateWidget<USATORI_MainUI>(GetGameInstance(), SATORIMainUI);
		if (SATORIMainWidget)
		{
			SATORIMainWidget->AddToViewport();
		}
	}*/
}

void ASATORI_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	/*if (SATORIMainUI)
	{
		SATORIMainWidget = CreateWidget<USATORI_MainUI>(GetGameInstance(), SATORIMainUI);
		if (SATORIMainWidget)
		{
			SATORIMainWidget->AddToViewport();
		}
	}*/
}

void ASATORI_PlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed
	
	CreateMainHUD();
}

void ASATORI_PlayerController::CreateMainHUD()
{
	if (SATORIMainUI)
	{
		SATORIMainWidget = CreateWidget<USATORI_MainUI>(GetGameInstance(), SATORIMainUI);
		if (SATORIMainWidget)
		{
			SATORIMainWidget->AddToViewport();
		}
	}
}