// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/Rangers/BTTask_MoveToRange.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToRange::UBTTask_MoveToRange()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_MoveToRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_RangeMovable* AI = Cast<ASATORI_RangeMovable>(Pawn);

	ASATORI_CharacterBase* Player1 = Cast<ASATORI_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	float dist = AI->GetDistanceTo(Player1);

	
	AActor* DESTINO = Cast<AActor>(Player1);
	if(Player1)
	{
		EPathFollowingRequestResult::Type Prueba = AIController->MoveToActor(Player1, AI->getDistAttack() - 50.0f,false);

		UBehaviorTreeComponent* OwnerComp2 = Cast<UBehaviorTreeComponent>(GetOuter());
		FinishLatentTask(*OwnerComp2, EBTNodeResult::Succeeded);
	
	}

	
	
	
	
	Result = EBTNodeResult::Succeeded;
	

	return Result;
}

EBTNodeResult::Type UBTTask_MoveToRange::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	Result = EBTNodeResult::Aborted;
	return Result;
}