// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_InRangeSpawned.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTDecorator_InRangeSpawned : public UBTDecorator
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MyBlackboardKey;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
