// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Spawner/SATORI_Spawner.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Controller.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "Data/SATORI_AbilityDataAsset.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"



ASATORI_Spawner::ASATORI_Spawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Spawner/BT_Spawner.BT_Spawner")));

}

void ASATORI_Spawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SpawnTime += DeltaSeconds;
}

void ASATORI_Spawner::ResetSpawnTime()
{
	SpawnTime = 0.f;
}

int ASATORI_Spawner::GetMaxEnemies() const
{
	return MaxNumSpawn;
}

void ASATORI_Spawner::AddNumEnemies(int Num)
{

	NumEnemies+=Num;
}

float ASATORI_Spawner::GetCoolDown() const
{
	return CoolDown;
}

int ASATORI_Spawner::GetNumEnemies()
{
	return NumEnemies;
}

float ASATORI_Spawner::GetSpawnTime()
{
	return SpawnTime;
}