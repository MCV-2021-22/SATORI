// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/BTDecorator_OutRangeMax.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
 * Calculate if AI is in range of target
 */
bool UBTDecorator_OutRangeMax::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_AICharacter* AI = Cast<ASATORI_AICharacter>(Pawn);

	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	float dist = AI->GetDistanceTo(Player1);

	if(dist > AI->getMaxRangeDist())
	{
		return true;
	}

	return false;

}