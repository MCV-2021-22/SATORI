// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTaksGenerales/BTTask_ActivateInmuneBosses.h"
#include "AIController.h"
#include "SATORICharacter.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ActivateInmuneBosses::UBTTask_ActivateInmuneBosses()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ActivateInmuneBosses::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Pawn);
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(Pawn);

	if(Fujin)
	{
		if(!activar)
		{
			if (!Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Inmune"))))
			{
				Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Inmune"));
			}
		}
		else
		{
			if (Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Inmune"))))
			{
				Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Inmune"));
			}
			if (Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos")))
			{
				Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos"));
			}
		}

		
	}
	else if (Raijin)
	{
		if (!activar)
		{
			if (!Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Inmune"))))
			{
				Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Inmune"));
			}
		}
		else
		{
			if (Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Inmune"))))
			{
				Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Inmune"));
			}
			if (Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos")))
			{
				Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos"));
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

EBTNodeResult::Type UBTTask_ActivateInmuneBosses::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	Result = EBTNodeResult::Aborted;
	return Result;
}