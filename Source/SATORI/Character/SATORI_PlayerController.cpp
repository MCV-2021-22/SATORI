// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_PlayerController.h"
#include "UI/SATORI_MainUI.h"

ASATORI_PlayerController::ASATORI_PlayerController(const class FObjectInitializer& InitializerObject)
	: Super(InitializerObject)
{

}

void ASATORI_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (SATORIMainUI)
	{
		SATORIMainWidget = CreateWidget<USATORI_MainUI>(GetGameInstance(), SATORIMainUI);
		if (SATORIMainWidget)
		{
			SATORIMainWidget->AddToViewport();
		}
	}
}