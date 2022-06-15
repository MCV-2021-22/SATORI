// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/SATORI_GameState.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "Data/SATORI_PortalPassiveDataAsset.h"

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

    FillPortalGameplayEffectWithData();

    for (int i = 0; i < InstancePortals.Num(); i++)
    {
        int RandomNumber = GenerateRandomNumberForPortal();
        FSATORI_DoorPassiveReward EffectToApply = PortalEffectsToApply[RandomNumber];
        InstancePortals[i]->SetCurrentGameplayEffectData(EffectToApply);
    }
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

void ASATORI_GameState::Tick(float DeltaSeconds)
{

}

int ASATORI_GameState::GenerateRandomNumberForPortal()
{
    const int EffectSize = PortalEffectsToApply.Num() - 1;
    int number = FMath::RandRange(0, EffectSize);
    return number;
}

void ASATORI_GameState::AddEnemyActor(AActor* Enemy)
{
    EnemyActors.Add(Enemy);
}

void ASATORI_GameState::RemoveEnemyActor(AActor* Enemy)
{
    EnemyActors.Remove(Enemy);
}

bool ASATORI_GameState::IsEmptyEnemyActors()
{
    return EnemyActors.Num() == 0 ? true : false;
}

