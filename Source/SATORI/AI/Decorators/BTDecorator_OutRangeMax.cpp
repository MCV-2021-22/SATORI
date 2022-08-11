// 

#include "AI/Decorators/BTDecorator_OutRangeMax.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
 * Calculate if AI is in range of target
 */
bool UBTDecorator_OutRangeMax::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) 
	{
		return false;
	}

	ASATORI_AICharacter* AI = Cast<ASATORI_AICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ASATORI_CharacterBase* Target = Cast<ASATORI_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	float Distance = AI->GetDistanceTo(Target);
	if(Distance > AI->GetMaxRange())
	{
		return true;
	}
	return false;
}