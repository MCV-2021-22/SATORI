//

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToWithRanges.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTTask_MoveToWithRanges : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_MoveToWithRanges();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector MyBlackboardKey;

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius;
	UPROPERTY(EditAnywhere)
		float MaxRadiusToFail;

	UPROPERTY(EditAnywhere)
		bool bStopOnOverlap;
	UPROPERTY(EditAnywhere)
		bool bUsePathfinding;
	UPROPERTY(EditAnywhere)
		bool bCanStrafe;
	UPROPERTY(EditAnywhere);
		TSubclassOf< UNavigationQueryFilter > FilterClass;
	UPROPERTY(EditAnywhere)
		bool bAllowPartialPath;

};
