// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Abilities/SATORI_ChangeAbilitiesWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"

bool USATORI_ChangeAbilitiesWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_ChangeAbilitiesWidget::ChangeAbilityIcons(UTexture2D* NewIcons)
{
	//if (AbilityIcons)
	//{
	//	AbilityIcons->SetBrushFromTexture(NewIcons);
	//}
}

void USATORI_ChangeAbilitiesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Character->PlayerGameplayAbilityComponent;
		if (CurrentAbilityComponent)
		{
			CurrentAbilityComponent->AbilityIconChange.AddDynamic(this, &USATORI_ChangeAbilitiesWidget::BP_OnData);
		}
	}
	//->GamePlayer->GetComponb->OnAbilityChanged.AddUObject(this, &USATORI_ChangeAbilitiesWidget::OnAbilityChanged)
}

void USATORI_ChangeAbilitiesWidget::NativeDestruct()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Character->PlayerGameplayAbilityComponent;
		if (CurrentAbilityComponent)
		{
			CurrentAbilityComponent->AbilityIconChange.RemoveAll(this); // , & USATORI_ChangeAbilitiesWidget::BP_OnData);
		}
	}
	//->GamePlayer->GetComponb->OnAbilityChanged.RemoveO
	Super::NativeDestruct();
}

