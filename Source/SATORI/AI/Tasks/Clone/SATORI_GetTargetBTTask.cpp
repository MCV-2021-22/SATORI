//

#include "AI/Tasks/Clone/SATORI_GetTargetBTTask.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SATORICharacter.h"
#include "SATORIGameMode.h"
//#include "GameState/SATORI_GameState.h"

USATORI_GetTargetBTTask::USATORI_GetTargetBTTask()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type USATORI_GetTargetBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	ASATORICharacter* Player = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Target = nullptr;
	float Range = CloneRange;

	if (Player->GetTargetSystemComponent()->IsLocked())
	{
		Target = Player->GetTargetSystemComponent()->GetLockedOnTargetActor();
	}
	else {

		AAIController* AIController = OwnerComp.GetAIOwner();
		APawn* Pawn = AIController->GetPawn();
		FVector ClonePosition = Pawn->GetActorLocation();
		TArray < AActor* >& AllEnemies = GetWorld()->GetAuthGameMode<ASATORIGameMode>()->GetEnemyActorsRef();
		//TArray < AActor* >& AllEnemies = GetWorld()->GetGameState<ASATORI_GameState>()->GetEnemyActorsRef();
		for (AActor* Actor : AllEnemies)
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
