// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Spawned/SATORI_Spawned.h"
#include "AIController.h"
#include "SATORIGameMode.h"

ASATORI_Spawned::ASATORI_Spawned()
{
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Spawned/BT_Spawned.BT_Spawned")));

	
}

void ASATORI_Spawned::SetMySpawn(ASATORI_Spawner* SpawnToSet)
{
	MySpawn = SpawnToSet;

}

void ASATORI_Spawned::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	int a = 1;

	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->AddEnemyActor(this);
}

float ASATORI_Spawned::GetDistAttack() const
{
	return DistAttack;
}

void ASATORI_Spawned::SpawnedDie()
{
	if(MySpawn)
	{
		MySpawn->AddNumEnemies(-1);
		this->Destroy();
		GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(this);
	}
	else
	{
		this->Destroy();
		GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(this);
	}
}