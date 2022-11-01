// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/Fujin/BTTask_ProbFujinCombo3.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ProbFujinCombo3::UBTTask_ProbFujinCombo3()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ProbFujinCombo3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Pawn);
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(Pawn);

	if(Fujin)
	{
		if (Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))))
		{
			TArray<float> Probs = Fujin->GetArrayProbs(ComboRaijin);

			if (Probs.Num() > 2)
			{
				if (Probs[2] - 5 >= 0)
				{
					for (int i = 0; i < Probs.Num(); i++)
					{
						if (i == 2)
						{
							Probs[i] += -5;
							UE_LOG(LogTemp, Warning, TEXT("New Probabilidad: %f, Combo: %d"), Probs[i], i+1);
						}
						else
						{
							Probs[i] += 2.5;
							UE_LOG(LogTemp, Warning, TEXT("New Probabilidad: %f, Combo: %d"), Probs[i], i + 1);
						}
					}

				}
				else if (Probs[2] > 0)
				{
					float dif = Probs[2] / 2;

					for (int i = 0; i < Probs.Num(); i++)
					{
						if (i == 2)
						{
							Probs[i] = 0;
							UE_LOG(LogTemp, Warning, TEXT("New Probabilidad: %f, Combo: %d"), Probs[i], i + 1);
						}
						else
						{
							Probs[i] += dif;
							UE_LOG(LogTemp, Warning, TEXT("New Probabilidad: %f, Combo: %d"), Probs[i], i + 1);
						}
					}


				}

				Fujin->SetArrayProbs(ComboRaijin, Probs);

			}

		}

	}
	

	//ASATORI_CharacterBase* Player1 = Cast<ASATORI_CharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(MyBlackboardKey.SelectedKeyName));

	/*float dist = AI->GetDistanceTo(Player1);

	
	AActor* DESTINO = Cast<AActor>(Player1);
	if(Player1)
	{
		EPathFollowingRequestResult::Type Prueba = AIController->MoveToActor(Player1, AI->getDistAttack() - 50.0f,false);

		UBehaviorTreeComponent* OwnerComp2 = Cast<UBehaviorTreeComponent>(GetOuter());
		FinishLatentTask(*OwnerComp2, EBTNodeResult::Succeeded);
	
	}*/

	
	
	
	
	Result = EBTNodeResult::Succeeded;
	

	return Result;
}

EBTNodeResult::Type UBTTask_ProbFujinCombo3::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	Result = EBTNodeResult::Aborted;
	return Result;
}