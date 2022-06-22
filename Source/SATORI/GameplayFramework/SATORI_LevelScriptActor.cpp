// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/SATORI_LevelScriptActor.h"
#include "Character/SATORI_CharacterBase.h"
#include "SATORICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayFramework/SATORI_GameInstance.h"

void ASATORI_LevelScriptActor::LoadPlayerStats()
{

	USATORI_GameInstance* GameInstanceRef = nullptr;
	ASATORICharacter* Character = nullptr;

	UWorld* World = GetWorld();
	if (World)
	{
		GameInstanceRef = Cast<USATORI_GameInstance>(World->GetGameInstance());
		Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}

	if (GameInstanceRef != nullptr && Character != nullptr)
	{
		Character->SetHealth(GameInstanceRef->PlayerLife);
		Character->SetMana(GameInstanceRef->PlayerMana);
		Character->SetDefense(GameInstanceRef->PlayerDefense);
		Character->AttributeSetBase = GameInstanceRef->Atributset;
		//Character->SetMaxHealth(GameInstanceRef->PlayerMaxLife);
		//Character->SetMaxMana(GameInstanceRef->PlayerMaxMana);
	}

}