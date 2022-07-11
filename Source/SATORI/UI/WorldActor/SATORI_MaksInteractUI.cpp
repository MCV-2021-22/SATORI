// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WorldActor/SATORI_MaksInteractUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"

bool USATORI_MaksInteractUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_MaksInteractUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (!isClicked)
	{
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
	}
}

void USATORI_MaksInteractUI::AkaMaskButtonOnClicked()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		isClicked = true;
		Character->SetCharacterMask(SATORIMaskType::Aka);
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
		if (PlayerController)
		{
			Character->EnableInput(PlayerController);
		}
	}
}

void USATORI_MaksInteractUI::AoMaskButtonOnClicked()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		isClicked = true;
		Character->SetCharacterMask(SATORIMaskType::Ao);
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
		if (PlayerController)
		{
			Character->EnableInput(PlayerController);
		}
	}
}

void USATORI_MaksInteractUI::MidoriMaskButtonOnClicked()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		isClicked = true;
		Character->SetCharacterMask(SATORIMaskType::Midori);
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
		if (PlayerController)
		{
			Character->EnableInput(PlayerController);
		}
	}
}