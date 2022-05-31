//

#include "AI/Tasks/Clone/SATORI_GetTargetBTTask.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

USATORI_GetTargetBTTask::USATORI_GetTargetBTTask()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type USATORI_GetTargetBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();
	FVector ClonePosition = Pawn->GetActorLocation();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORI_AICharacter::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		ASATORI_CharacterBase* Character = Cast<ASATORI_AICharacter>(Actor);
		if (Character->HasMatchingGameplayTag(TargetActorWithTag))
		{
			Target = Actor;
		}
		else
		{
			if (!Character->HasMatchingGameplayTag(CloneTag))
			{
				const float Distance = FVector::Distance(Actor->GetActorLocation(), ClonePosition);
				if (Distance < Range)
				{
					Range = Distance;
					TargetNear = Actor;
				}
			}
		}
	}

	if (!Target && TargetNear)
	{
		Target = TargetNear;
	}

	if (Target)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName, Target);

		Result = EBTNodeResult::Succeeded;
	}

	return Result;
}
