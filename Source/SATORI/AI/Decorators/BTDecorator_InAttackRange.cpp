//

#include "AI/Decorators/BTDecorator_InAttackRange.h"
#include "AIController.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

/*
 * Calculate if AI is in range of target
 */
bool UBTDecorator_InAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess)
	{
		return false;
	}

	ASATORI_AICharacter* AI = Cast<ASATORI_AICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ASATORI_CharacterBase* Target = Cast<ASATORI_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	return AI->GetDistanceTo(Target) <= AI->GetAttackDistance();
}