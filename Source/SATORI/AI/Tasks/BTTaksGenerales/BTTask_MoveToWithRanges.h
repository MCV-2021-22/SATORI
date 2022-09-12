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
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector MyBlackboardKey;

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 0.0f;
	UPROPERTY(EditAnywhere)
		float MaxRadiusToFail = 0.0f;

	UPROPERTY(EditAnywhere)
		bool bStopOnOverlap = true;
	UPROPERTY(EditAnywhere)
		bool bUsePathfinding = true;
	UPROPERTY(EditAnywhere)
		bool bCanStrafe;
	UPROPERTY(EditAnywhere);
		TSubclassOf< UNavigationQueryFilter > FilterClass;
	UPROPERTY(EditAnywhere)
		bool bAllowPartialPath = true;

};
