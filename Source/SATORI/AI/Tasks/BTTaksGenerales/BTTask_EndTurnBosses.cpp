// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTaksGenerales/BTTask_EndTurnBosses.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EndTurnBosses::UBTTask_EndTurnBosses()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_EndTurnBosses::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Pawn);
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(Pawn);

	if(Fujin)
	{
		Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Turno.Fujin"));

		if(Fujin->Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))) || Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))))
		{
			Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Listos")));
			Fujin->Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Listos")));

			if(Fujin->Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Raijin"))))
			{
				Fujin->Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Raijin")));
			}

		}
		else
		{
			Fujin->Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Turno.Raijin"));
		}
		

	}
	else if (Raijin)
	{
		Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Turno.Raijin"));
		if (Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))) || Raijin->Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))))
		{
			Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Listos")));
			Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Listos")));
			if (Raijin->Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Fujin"))))
			{
				Raijin->Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Fujin")));
			}

		}
		else
		{
			Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Turno.Fujin"));
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

EBTNodeResult::Type UBTTask_EndTurnBosses::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	Result = EBTNodeResult::Aborted;
	return Result;
}