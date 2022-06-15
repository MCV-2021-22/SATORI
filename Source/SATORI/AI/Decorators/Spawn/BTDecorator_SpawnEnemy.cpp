// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Spawn/BTDecorator_SpawnEnemy.h"

#include "AIController.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/Spawner/SATORI_Spawner.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_SpawnEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Spawner* AI = Cast<ASATORI_Spawner>(Pawn);

	int NumEnemies = AI->GetNumEnemies();

	float Time = AI->GetSpawnTime();

	float MaxTime = AI->GetCoolDown();


	if (NumEnemies < AI->GetMaxEnemies() && Time >= MaxTime)
	{
		return true;
	}
	return false;
}
