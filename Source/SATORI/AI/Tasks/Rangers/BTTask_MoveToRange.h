// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToRange.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTTask_MoveToRange : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_MoveToRange();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector MyBlackboardKey;

};
