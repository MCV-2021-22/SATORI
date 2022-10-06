// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Raijin/BTDecorator_PlayerClose.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "BehaviorTree/BlackboardComponent.h"


bool UBTDecorator_PlayerClose::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Raijin* AI = Cast<ASATORI_Raijin>(Pawn);

	ASATORI_CharacterBase* Player1 = Cast<ASATORI_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	if(Player1)
	{
		float dist = AI->GetDistanceTo(Player1);
		UE_LOG(LogTemp, Warning, TEXT("Dist %f"), dist);
		if (dist <= AI->getSoCloseDist())
		{
			return true;
		}
	}

	return false;

}