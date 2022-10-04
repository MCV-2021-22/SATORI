// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SATORI_CheatManager.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"

void USATORI_CheatManager::UnlockAllAbilities()
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character)
	{
		USATORI_GameplayAbilityComponent* AbilityComp = Character->GetPlayerAbilityComponent();
		if (AbilityComp)
		{
			AbilityComp->UnclockAllHabilities(true);
		}
	}
}