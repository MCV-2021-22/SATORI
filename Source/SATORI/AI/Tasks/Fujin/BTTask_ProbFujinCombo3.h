// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ProbFujinCombo3.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTTask_ProbFujinCombo3 : public UBTTaskNode
{
	GENERATED_BODY()

		UBTTask_ProbFujinCombo3();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int ComboRaijin = 0;

	


};
