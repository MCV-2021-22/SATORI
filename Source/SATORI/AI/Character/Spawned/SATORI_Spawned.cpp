// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Spawned/SATORI_Spawned.h"
#include "AIController.h"

ASATORI_Spawned::ASATORI_Spawned()
{
	PrimaryActorTick.bCanEverTick = true;

	bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Spawned/BT_Spawned.BT_Spawned")));

	
}

void ASATORI_Spawned::SetMySpawn(ASATORI_Spawner* SpawnToSet)
{
	MySpawn = SpawnToSet;

}

void ASATORI_Spawned::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	int a = 1;
}

float ASATORI_Spawned::GetDistAttack() const
{
	return DistAttack;
}

float ASATORI_Spawned::SpawnedDie()
{

	int a = 2;

	if(MySpawn)
	{
		MySpawn->AddNumEnemies(-1);
		return 2.0f;
	}
	//
	return 0.1;
}