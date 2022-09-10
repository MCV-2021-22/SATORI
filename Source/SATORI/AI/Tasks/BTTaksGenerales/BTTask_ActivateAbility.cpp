// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTaksGenerales/BTTask_ActivateAbility.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemGlobals.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
	bool bActivationSuccessful = ASC->TryActivateAbilityByClass(AbilityToActivate);
	
	if (bActivationSuccessful)
	{
		FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(AbilityToActivate);
		UGameplayAbility* AbilityInstance = AbilitySpec->GetPrimaryInstance();
		if (AbilityInstance->IsActive())
		{
			AbilityInstance->OnGameplayAbilityEnded.AddUObject(this, &UBTTask_ActivateAbility::OnAbilityEnded);
			Result = EBTNodeResult::InProgress;
		}
		else
		{
			Result = EBTNodeResult::Succeeded;
		}
		
	}

	return Result;
}

void UBTTask_ActivateAbility::OnAbilityEnded(UGameplayAbility* EndedAbility)
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
