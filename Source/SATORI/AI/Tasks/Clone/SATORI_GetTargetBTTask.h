//

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SATORI_GetTargetBTTask.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_GetTargetBTTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	USATORI_GetTargetBTTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clone")
	float CloneRange;

private:

	AActor* Target;

};
