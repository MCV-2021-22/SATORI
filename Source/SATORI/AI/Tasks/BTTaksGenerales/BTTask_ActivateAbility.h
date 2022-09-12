// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbility.generated.h"

class UGameplayAbility;

/**
 * Activate an abiltiy using a Behavior Tree class
 */
UCLASS()
class SATORI_API UBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_ActivateAbility();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> AbilityToActivate;

	UFUNCTION()
	void OnAbilityEnded(UGameplayAbility* EndedAbility);
};
