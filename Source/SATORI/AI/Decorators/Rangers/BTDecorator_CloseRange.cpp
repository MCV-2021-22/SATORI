// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Rangers/BTDecorator_CloseRange.h"

#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_CloseRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_RangeMovable* AI = Cast<ASATORI_RangeMovable>(Pawn);


	
	ASATORI_CharacterBase* Player1 = Cast<ASATORI_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	float dist = AI->GetDistanceTo(Player1);

	if(dist<= AI->getCloseDist())
	{
		return true;
	}

	return false;

}
