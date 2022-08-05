// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/SATORI_GameState.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "Data/SATORI_PortalPassiveDataAsset.h"
#include "Data/SATORI_PortalGrantedAbilityAsset.h"
#include "GameplayFramework/SATORI_GameInstance.h"

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

   /* GeneratedRandomPlayerAbility();
    GeneratedRandomPassiveEffect();*/
    GenerateRandomPassiveEffectAndAbilities();
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
    GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstanceRef)
    {
        PortalGrantedUpgratedAbilityToApply = GameInstanceRef->GetPortalGrantedUpgratedAbility();
        PortalGrantedNormalAbilityToApply = GameInstanceRef->GetPortalGrantedNormalAbility();
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

void ASATORI_GameState::GenerateRandomPassiveEffectAndAbilities()
{
    if (InstancePortals.Num() == 1)
        return;

    ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Character)
    {
        for (int i = 0; i < InstancePortals.Num(); i++)
        {
            // Generate random gameplayEffect
            int RandomNumber = GenerateRandomNumberForPortal();
            FSATORI_DoorPassiveReward EffectToApply = PortalEffectsToApply[RandomNumber];
            InstancePortals[i]->SetCurrentGameplayEffectData(EffectToApply);

            // Generate random abilities
            // Check if there have more abilities
            if (PortalGrantedUpgratedAbilityToApply.Num() > 0 || PortalGrantedNormalAbilityToApply.Num() > 0)
            {
                // Upgrated Abilities
                if (Character->GetIsAbilityUpgrated())
                {
                    int Size = PortalGrantedUpgratedAbilityToApply.Num() - 1;
                    FSATORI_PortalAbilitiesDatasReward Reward = PortalGrantedUpgratedAbilityToApply[Size];
                    InstancePortals[i]->SetCurrentGameplayAbilityData(Reward);
                    UE_LOG(LogTemp, Display, TEXT(" Player Upgrated Ability Size : [%d] "), Size);
                }
                // Normal Abilities
                else
                {
                    int Size = PortalGrantedNormalAbilityToApply.Num() - 1;
                    FSATORI_PortalAbilitiesDatasReward Reward = PortalGrantedNormalAbilityToApply[Size];
                    InstancePortals[i]->SetCurrentGameplayAbilityData(Reward);
                    UE_LOG(LogTemp, Display, TEXT(" Player Normal Abilities Size : [%d] "), Size);
                    FString AbilityString = Reward.AbilityName.ToString();
                    UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
                }
            }
        }
    }
}

void ASATORI_GameState::GeneratedRandomPassiveEffect()
{
    if (InstancePortals.Num() == 1)
        return;

    ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    for (int i = 1; i < InstancePortals.Num(); i++)
    {
        int RandomNumber = GenerateRandomNumberForPortal();
        FSATORI_DoorPassiveReward EffectToApply = PortalEffectsToApply[RandomNumber];
        InstancePortals[i]->SetCurrentGameplayEffectData(EffectToApply);
    }

    // IF no more ability left
    if (PortalGrantedUpgratedAbilityToApply.Num() <= 0 || PortalGrantedNormalAbilityToApply.Num() <= 0)
    {
        int RandomNumber = GenerateRandomNumberForPortal();
        FSATORI_DoorPassiveReward EffectToApply = PortalEffectsToApply[RandomNumber];
        InstancePortals[0]->SetCurrentGameplayEffectData(EffectToApply);
    }
}

void ASATORI_GameState::GeneratedRandomPlayerAbility()
{
    /*ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Character)
    {
        if (Character->GetIsAbilityUpgrated() && PortalGrantedUpgratedAbilityToApply.Num() > 0)
        {
            int Size = PortalGrantedUpgratedAbilityToApply.Num() - 1;
            FSATORI_PortalAbilitiesDatasReward Reward = PortalGrantedUpgratedAbilityToApply[Size];
            InstancePortals[0]->SetCurrentGameplayAbilityData(Reward);
            UE_LOG(LogTemp, Display, TEXT(" Player Upgrated Ability Size : [%d] "), Size);
            if (GameInstanceRef)
            {
                GameInstanceRef->RemoveElementonFromUpgratedAbilities(1);
            }
        }
        else (PortalGrantedNormalAbilityToApply.Num() > 0)
        {
            int Size = PortalGrantedNormalAbilityToApply.Num() - 1;
            FSATORI_PortalAbilitiesDatasReward Reward = PortalGrantedNormalAbilityToApply[Size];
            InstancePortals[0]->SetCurrentGameplayAbilityData(Reward);
            UE_LOG(LogTemp, Display, TEXT(" Player Normal Abilities Size : [%d] "), Size);
            FString AbilityString = Reward.AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            if (GameInstanceRef)
            {
                GameInstanceRef->RemoveElementonFromNormalAbilities(1);
            }
        }
    }*/
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

