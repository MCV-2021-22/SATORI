//

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetTarget.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API UBTTask_GetTarget: public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_GetTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clone")
	float CloneRange;

private:

	AActor* Target;

};
