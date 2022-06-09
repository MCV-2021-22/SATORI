//

#include "AI/Tasks/Clone/SATORI_GetTargetBTTask.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SATORICharacter.h"

USATORI_GetTargetBTTask::USATORI_GetTargetBTTask()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type USATORI_GetTargetBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	ASATORICharacter* Player = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player->GetTargetSystemComponent()->IsLocked())
	{
		Target = Player->GetTargetSystemComponent()->GetLockedOnTargetActor();
	}
	else {

		AAIController* AIController = OwnerComp.GetAIOwner();
		APawn* Pawn = AIController->GetPawn();
		FVector ClonePosition = Pawn->GetActorLocation();

		//Range = OwnerComp.GetBlackboardComponent()->GetKeyID("Range");

		TArray<AActor*> Actors = Player->GetTargetSystemComponent()->GetTargetableActors();
		for (AActor* Actor : Actors)
		{
			const float Distance = FVector::Distance(ClonePosition, Actor->GetActorLocation());
			if (Distance < Range)
			{
				Range = Distance;
				Target = Actor;
			}
		}
	}

	if (Target)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName, Target);

		Result = EBTNodeResult::Succeeded;
	}

	return Result;
}
