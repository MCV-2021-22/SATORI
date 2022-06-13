// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/SATORI_GameState.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "Kismet/GameplayStatics.h"

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
}

void ASATORI_GameState::Tick(float DeltaSeconds)
{

}
