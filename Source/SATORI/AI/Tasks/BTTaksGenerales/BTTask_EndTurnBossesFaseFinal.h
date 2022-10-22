// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EndTurnBossesFaseFinal.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTTask_EndTurnBossesFaseFinal : public UBTTaskNode
{
	GENERATED_BODY()

		UBTTask_EndTurnBossesFaseFinal();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector MyBlackboardKey;

	


};
