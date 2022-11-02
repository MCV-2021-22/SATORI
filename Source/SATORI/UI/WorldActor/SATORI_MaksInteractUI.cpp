// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldActor/SATORI_MaksInteractUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "UI/Menu/PopUp/SATORI_MaskVendorPopUp.h"

bool USATORI_MaksInteractUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	AkaPopUp->SetVisibility(ESlateVisibility::Hidden);
	AoPopUp->SetVisibility(ESlateVisibility::Hidden);
	MidoriPopUp->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void USATORI_MaksInteractUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (AkaButton)
	{
		AkaButton->OnClicked.AddDynamic(this, &USATORI_MaksInteractUI::AkaMaskButtonOnClicked);
	}

	if (AoButton)
	{
		AoButton->OnClicked.AddDynamic(this, &USATORI_MaksInteractUI::AoMaskButtonOnClicked);
	}

	if (MidoriButton)
	{
		MidoriButton->OnClicked.AddDynamic(this, &USATORI_MaksInteractUI::MidoriMaskButtonOnClicked);
	}

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &USATORI_MaksInteractUI::OnResumeClicked);
	}
}

void USATORI_MaksInteractUI::AkaMaskButtonOnClicked()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		isClicked = true;
		if (Character->MaskType != SATORIMaskType::NONE)
		{
			Character->RemoveMaskGameplayEffect();
		}
		Character->SetCharacterMask(SATORIMaskType::Aka);
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
	}

	// Pop up
	if (AkaPopUp)
	{
		AkaPopUp->SetVisibility(ESlateVisibility::Visible);
	}
}

void USATORI_MaksInteractUI::AoMaskButtonOnClicked()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		isClicked = true;
		if (Character->MaskType != SATORIMaskType::NONE)
		{
			Character->RemoveMaskGameplayEffect();
		}
		Character->SetCharacterMask(SATORIMaskType::Ao);
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
	}

	// Pop up
	if (AoPopUp)
	{
		AoPopUp->SetVisibility(ESlateVisibility::Visible);
	}
}

void USATORI_MaksInteractUI::MidoriMaskButtonOnClicked()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		isClicked = true;
		if (Character->MaskType != SATORIMaskType::NONE)
		{
			Character->RemoveMaskGameplayEffect();
		}
		Character->SetCharacterMask(SATORIMaskType::Midori);
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
	}

	// Pop up
	if (MidoriPopUp)
	{
		MidoriPopUp->SetVisibility(ESlateVisibility::Visible);
	}
}

void USATORI_MaksInteractUI::OnResumeClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	this->RemoveFromParent();
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			Character->EnableInput(PlayerController);
		}
	}
}