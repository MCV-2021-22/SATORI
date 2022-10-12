// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Fujin//BTDecorator_FujinRandomAb.h"
#include "AIController.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"



bool UBTDecorator_FujinRandomAb::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Fujin* AI = Cast<ASATORI_Fujin>(Pawn);

	if (AI)
	{
		// 25%
		int randomnum = rand() % 100 + 1;

		if (randomnum < Probability)
		{
			return true;
		}
	}


	return false;

}