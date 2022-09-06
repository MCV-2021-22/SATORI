// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldActor/SATORI_GeneralVendorInteractUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "GameState/SATORI_GameState.h"

bool USATORI_GeneralVendorInteractUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_GeneralVendorInteractUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuyItemButton)
	{
		BuyItemButton->OnClicked.AddDynamic(this, &USATORI_GeneralVendorInteractUI::BuyItem);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &USATORI_GeneralVendorInteractUI::ClosePanel);
	}
}

void USATORI_GeneralVendorInteractUI::BuyItem()
{
	if (!isClicked)
	{
		isClicked = true;

		ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (SatoriCharacter)
		{
			SatoriCharacter->SetIsAbilityUpgrated(true);			
		}
		// Reset portal abilities
		ASATORI_GameState* GameState = GetWorld() != NULL ? GetWorld()->GetGameState<ASATORI_GameState>() : NULL;
		if (GameState)
		{
			GameState->ResetAllPortalAbilities();
		}
	}
}

void USATORI_GeneralVendorInteractUI::ClosePanel()
{
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ASATORI_PlayerController* PlayerController = nullptr;
	if (SatoriCharacter)
	{
		PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			SatoriCharacter->EnableInput(PlayerController);
		}
	}

	this->RemoveFromViewport();
}


