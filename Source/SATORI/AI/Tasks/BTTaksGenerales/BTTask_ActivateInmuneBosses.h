// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateInmuneBosses.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTTask_ActivateInmuneBosses : public UBTTaskNode
{
	GENERATED_BODY()

		UBTTask_ActivateInmuneBosses();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	

	UPROPERTY(EditAnywhere)
		bool activar = true;

	


};
