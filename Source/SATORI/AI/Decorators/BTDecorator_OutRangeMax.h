// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_OutRangeMax.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTDecorator_OutRangeMax : public UBTDecorator
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MyBlackboardKey;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
