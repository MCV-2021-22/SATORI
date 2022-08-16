//

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TooFarAway.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTDecorator_TooFarAway : public UBTDecorator
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MyBlackboardKey;

	UPROPERTY(EditAnywhere)
	float PlusDistanceToMaxRange;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
