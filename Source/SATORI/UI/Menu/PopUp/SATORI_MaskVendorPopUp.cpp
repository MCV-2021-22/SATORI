// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PopUp/SATORI_MaskVendorPopUp.h"
#include "Components/Button.h"

bool USATORI_MaskVendorPopUp::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_MaskVendorPopUp::NativeConstruct()
{
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &USATORI_MaskVendorPopUp::OnWindowClosed);
	}
}

void USATORI_MaskVendorPopUp::NativeDestruct()
{

}

void USATORI_MaskVendorPopUp::OnWindowClosed()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}