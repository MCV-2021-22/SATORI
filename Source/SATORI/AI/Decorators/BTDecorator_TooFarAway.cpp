// 

#include "AI/Decorators/BTDecorator_TooFarAway.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
 * Calculate if AI too far away from target
 */
bool UBTDecorator_TooFarAway::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) 
	{
		return false;
	}

	ASATORI_AICharacter* AI = Cast<ASATORI_AICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ASATORI_CharacterBase* Target = Cast<ASATORI_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	float Distance = AI->GetDistanceTo(Target);
	if(Distance > AI->GetMaxRange() + PlusDistanceToMaxRange)
	{
		return true;
	}
	return false;
}