// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Raijin/BTDecorator_RaijinRandomAb.h"
#include "AIController.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"



bool UBTDecorator_RaijinRandomAb::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Raijin* AI = Cast<ASATORI_Raijin>(Pawn);

	if(AI)
	{
		// 25%
		int randomnum = rand() % 4 + 1;
					
		if (randomnum == 1)
		{
			return true;
		}
	}
	

	return false;

}