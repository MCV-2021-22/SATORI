// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SATORI_CheatManager.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SATORI_CharacterBase.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"

void USATORI_CheatManager::UnlockAllAbilities(bool Value)
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* AbilityComp = Character->GetPlayerAbilityComponent();
		if (AbilityComp)
		{
			AbilityComp->UnclockAllHabilities(Value);
		}
	}
}

void USATORI_CheatManager::SetEasyMode(bool Value)
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->SetEasyMode(Value);
	}
}

void USATORI_CheatManager::IncreasedAttack()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->WeaponDamage = 1000.0f;
	}
}

void USATORI_CheatManager::ShowMainUI()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_ComboSystemComponent* ComboComp = Character->GetComboSystemComponent();
		if (ComboComp)
		{
			ComboComp->SetMainWidgetVisibility(true);
		}
	}
}

void USATORI_CheatManager::SetPlayerHealthToOne()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->SetHealth(1);
	}
}

void USATORI_CheatManager::PlayerInstaDead()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		Character->CharacterDeath();
	}
}