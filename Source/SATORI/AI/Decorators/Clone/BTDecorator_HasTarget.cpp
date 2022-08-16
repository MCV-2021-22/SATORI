//

#include "AI/Decorators/Clone/BTDecorator_HasTarget.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_HasTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess)
	{
		return false;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName));

	if (Target)
	{
		return true;
	}
	return false;
}