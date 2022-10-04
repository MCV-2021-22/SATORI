// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SATORI_CheatManager.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SATORI_CharacterBase.h"

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