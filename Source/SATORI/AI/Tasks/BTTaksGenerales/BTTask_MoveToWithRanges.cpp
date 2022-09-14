//

#include "AI/Tasks/BTTaksGenerales/BTTask_MoveToWithRanges.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToWithRanges::UBTTask_MoveToWithRanges()
{
	bNotifyTick = true;
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

	AIController->MoveToActor(Target, AcceptanceRadius, bStopOnOverlap, bUsePathfinding, bCanStrafe, FilterClass, bAllowPartialPath);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_MoveToWithRanges::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{ 
		Character->GetCharacterMovement()->StopActiveMovement();
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	return EBTNodeResult::Aborted;
}

void UBTTask_MoveToWithRanges::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (IsValid(Target) && Character)
	{
		if (FVector::Dist(Character->GetActorLocation(), Target->GetActorLocation()) > MaxRadiusToFail)
		{
			Character->GetCharacterMovement()->StopActiveMovement();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
		if (FVector::Dist(Character->GetActorLocation(), Target->GetActorLocation()) < AcceptanceRadius)
		{
			Character->GetCharacterMovement()->StopActiveMovement();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
