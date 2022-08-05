// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Data/SATORI_PortalGrantedAbilityAsset.h"

USATORI_GameInstance::USATORI_GameInstance()
{

}

void USATORI_GameInstance::Init()
{
	Super::Init();
	InitSaveGame();
	FillPortalGrantedAbilityWithData();
}

void USATORI_GameInstance::Shutdown()
{
	Super::Shutdown();
	SaveGameToDisk();
}

void USATORI_GameInstance::SaveGameToDisk()
{
	
}

void USATORI_GameInstance::InitSaveGame()
{
	SaveGame = Cast<USATORI_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveGame)
	{
		SaveGame = Cast<USATORI_SaveGame>(UGameplayStatics::CreateSaveGameObject(USATORI_SaveGame::StaticClass()));
	}
}

void USATORI_GameInstance::RegisterActorToSave()
{

}

void USATORI_GameInstance::UnRegisterActorToSave()
{

}

void USATORI_GameInstance::FillPortalGrantedAbilityWithData()
{
    if (GrantedAbilityDataAsset)
    {
        for (const FPortalGrantedAbilityDatas& Data : GrantedAbilityDataAsset->GrantedAbilities)
        {
            FString CurrentName = Data.PortalGrantedAbilityName.ToString();
            FName LocalAbilityName = FName(*CurrentName);

            if (LocalAbilityName.IsValid() && Data.PortalGrantedAbility)
            {
                FSATORI_PortalAbilitiesDatasReward GrantedAbility;
                GrantedAbility.AbilitiyIcon = Data.PortalGrantedAbilitiyIcon;
                GrantedAbility.CurrentAbility = Data.PortalGrantedAbility;
                GrantedAbility.AbilityName = Data.PortalGrantedAbilityName;
                GrantedAbility.isUpgrated = Data.isUpgrated;
                if (GrantedAbility.isUpgrated)
                {
                    PortalGrantedUpgratedAbilityToApply.Add(GrantedAbility);
                    PortalRecicledUpgratedAbility.Add(GrantedAbility);
                }
                else
                {
                    PortalGrantedNormalAbilityToApply.Add(GrantedAbility);
                    PortalRecicledNormalAbility.Add(GrantedAbility);
                }
            }
        }
    }
}

void USATORI_GameInstance::RemoveElementonFromNormalAbilities(int Id)
{
    if (PortalGrantedNormalAbilityToApply.Num() > 0)
    {
        int Size = PortalGrantedNormalAbilityToApply.Num() - Id;
        if (Size < 0)
        {
            Size = 0;
        }
        if (PortalGrantedNormalAbilityToApply.Num() == 2 && Id == 1)
        {
            FString AbilityString = PortalGrantedNormalAbilityToApply[Size].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedNormalAbilityToApply.RemoveAt(0);
        }
        else if (PortalGrantedNormalAbilityToApply.Num() == 2 && Id == 2)
        {
            FString AbilityString = PortalGrantedNormalAbilityToApply[Size].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedNormalAbilityToApply.RemoveAt(1);
        }
        else
        {
            FString AbilityString = PortalGrantedNormalAbilityToApply[Size].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedNormalAbilityToApply.RemoveAt(Size);
        }
    }
}

void USATORI_GameInstance::RemoveElementonFromUpgratedAbilities(int Id)
{
    if (PortalGrantedUpgratedAbilityToApply.Num() > 0)
    {
        int Size = PortalGrantedUpgratedAbilityToApply.Num() - Id;
        if (Size < 0)
        {
            Size = 0;
        }
        if (PortalGrantedUpgratedAbilityToApply.Num() == 2 && Id == 1)
        {
            FString AbilityString = PortalGrantedUpgratedAbilityToApply[Size].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedUpgratedAbilityToApply.RemoveAt(0);
        }
        else if (PortalGrantedUpgratedAbilityToApply.Num() == 2 && Id == 2)
        {
            FString AbilityString = PortalGrantedUpgratedAbilityToApply[Size].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedUpgratedAbilityToApply.RemoveAt(1);
        }
        else
        {
            FString AbilityString = PortalGrantedNormalAbilityToApply[Size].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedNormalAbilityToApply.RemoveAt(Size);
        }
    }
}

void USATORI_GameInstance::ResetPortalRewardAbilities()
{
    // Clear the player Abilities
    NormalAbilities.Empty();
    UpgratedAbilities.Empty();

    // Clear the Array
    PortalGrantedUpgratedAbilityToApply.Empty();
    PortalGrantedNormalAbilityToApply.Empty();

    // Allocate the recicled Array elements to portal Array
    PortalGrantedUpgratedAbilityToApply = PortalRecicledUpgratedAbility;
    PortalGrantedNormalAbilityToApply = PortalRecicledNormalAbility;
}