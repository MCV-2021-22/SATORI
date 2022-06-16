// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Raijin/BTDecorator_OutOfSpawn.h"
#include "AIController.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"



bool UBTDecorator_OutOfSpawn::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Raijin* AI = Cast<ASATORI_Raijin>(Pawn);

	FVector vector_dist = AI->GetActorLocation() - AI->getSpawnPos();

	float dist = sqrt((pow(vector_dist.X, 2) + pow(vector_dist.Y, 2) + pow(vector_dist.Z, 2)));
	
	if(dist > AI->getSoCloseDist())
	{
		return true;
	}

	return false;

}