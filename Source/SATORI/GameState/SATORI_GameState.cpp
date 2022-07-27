// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/SATORI_GameState.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "Data/SATORI_PortalPassiveDataAsset.h"
#include "Data/SATORI_PortalGrantedAbilityAsset.h"

ASATORI_GameState::ASATORI_GameState()
{

}


// Called when the game starts or when spawned
void ASATORI_GameState::BeginPlay()
{
    TSubclassOf<ASATORI_Portal> classToFind;
    classToFind = ASATORI_Portal::StaticClass();
    TArray<AActor*> FoundPortals;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, FoundPortals);

    for (int i = 0; i < FoundPortals.Num(); i++)
    {
        ASATORI_Portal* Portal = Cast<ASATORI_Portal>(FoundPortals[i]);
        if (Portal)
        {
            InstancePortals.Add(Portal);
        }
    }

    // Fill datas
    FillPortalGameplayEffectWithData();
    FillPortalGrantedAbilityWithData();

    GeneratedRandomPassiveEffect();
    GeneratedRandomPlayerAbility();
}

void ASATORI_GameState::FillPortalGameplayEffectWithData()
{
    if (PassiveDataAsset)
    {
        for (const FSATORI_DoorPassiveDatas Data : PassiveDataAsset->PassiveRewards)
        {
            FString CurrentName = Data.Description.ToString();
            FName LocalAbilityName = FName(*CurrentName);

            if (LocalAbilityName.IsValid() && Data.PassiveEffect)
            {
                // Adding datas to map
                FSATORI_DoorPassiveReward PassiveReward;
                PassiveReward.PassiveEffect = Data.PassiveEffect;
                PassiveReward.PassiveIcon = Data.PassiveIcon;
                PassiveReward.Description = Data.Description;
                PortalEffectsToApply.Add(PassiveReward);
            }
        }
    }
}

void ASATORI_GameState::FillPortalGrantedAbilityWithData()
{
    if (GrantedAbilityDataAsset)
    {
        for (const FPortalGrantedAbilityDatas Data : GrantedAbilityDataAsset->GrantedAbilities)
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
                PortalGrantedAbilityToApply.Add(GrantedAbility);
            }
        }
    }
}

void ASATORI_GameState::Tick(float DeltaSeconds)
{

}

int ASATORI_GameState::GenerateRandomNumberForPortal()
{
    const int EffectSize = PortalEffectsToApply.Num() - 1;
    int number = FMath::RandRange(0, EffectSize);
    return number;
}

void ASATORI_GameState::GeneratedRandomPassiveEffect()
{
    if (InstancePortals.Num() == 1)
        return;

    for (int i = 1; i < InstancePortals.Num(); i++)
    {
        int RandomNumber = GenerateRandomNumberForPortal();
        FSATORI_DoorPassiveReward EffectToApply = PortalEffectsToApply[RandomNumber];
        InstancePortals[i]->SetCurrentGameplayEffectData(EffectToApply);
    }

    // IF no more ability left
    if (PortalGrantedAbilityToApply.Num() < 0)
    {
        int RandomNumber = GenerateRandomNumberForPortal();
        FSATORI_DoorPassiveReward EffectToApply = PortalEffectsToApply[RandomNumber];
        InstancePortals[InstancePortals.Num()]->SetCurrentGameplayEffectData(EffectToApply);
    }
}

void ASATORI_GameState::GeneratedRandomPlayerAbility()
{
    if (PortalGrantedAbilityToApply.Num() > 0)
    {
        int Size = PortalGrantedAbilityToApply.Num() - 1;
        FSATORI_PortalAbilitiesDatasReward Reward = PortalGrantedAbilityToApply[Size];
        InstancePortals[0]->SetCurrentGameplayAbilityData(Reward);
        PortalGrantedAbilityToApply.RemoveAt(PortalGrantedAbilityToApply.Num() - 1);
    }
}

//void ASATORI_GameState::AddEnemyActor(AActor* Enemy)
//{
//    EnemyActors.Add(Enemy);
//}
//
//void ASATORI_GameState::RemoveEnemyActor(AActor* Enemy)
//{
//    EnemyActors.Remove(Enemy);
//}
//
//bool ASATORI_GameState::IsEmptyEnemyActors()
//{
//    return EnemyActors.Num() == 0 ? true : false;
//}

