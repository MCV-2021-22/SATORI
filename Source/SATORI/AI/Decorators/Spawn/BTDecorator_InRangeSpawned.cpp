// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Spawn/BTDecorator_InRangeSpawned.h"

#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/Spawned/SATORI_Spawned.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_InRangeSpawned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Spawned* AI = Cast<ASATORI_Spawned>(Pawn);

	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	float dist = AI->GetDistanceTo(Player1);
	float distAI = AI->GetDistAttack();

	if(dist<= distAI)
	{
		return true;
	}

	return false;

}
