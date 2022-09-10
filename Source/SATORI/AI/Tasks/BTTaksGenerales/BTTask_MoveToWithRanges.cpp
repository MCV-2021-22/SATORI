//

#include "AI/Tasks/BTTaksGenerales/BTTask_MoveToWithRanges.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToWithRanges::UBTTask_MoveToWithRanges()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_MoveToWithRanges::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	if (!AIController || !Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]UBTTask_MoveToWithRanges: Cannot get Owner or Target ... "), *GetName());
		return EBTNodeResult::Failed;
	}

	EPathFollowingRequestResult::Type Moving = AIController->MoveToActor(Target, AcceptanceRadius, bStopOnOverlap, bUsePathfinding, bCanStrafe, FilterClass, bAllowPartialPath);

	if(Moving == EPathFollowingRequestResult::RequestSuccessful || Moving == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_MoveToWithRanges::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{ 
		Character->GetCharacterMovement()->StopActiveMovement();
	}
	return EBTNodeResult::Aborted;
}