// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Tasks/SATORI_AbilityTask_StartAbilityAndWait.h"
#include "AbilitySystemComponent.h"

UAbilityTask_StartAbilityAndWait* UAbilityTask_StartAbilityAndWait::StartAbilityAndWaitForEnd(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<UGameplayAbility> AbilityClassToActivate)
{
	UAbilityTask_StartAbilityAndWait* AbilityTask = NewAbilityTask<UAbilityTask_StartAbilityAndWait>(OwningAbility);
	AbilityTask->AbilityClassToActivate = AbilityClassToActivate;
	return AbilityTask;
}

void UAbilityTask_StartAbilityAndWait::Activate()
{
	bHasBeenActivated = AbilitySystemComponent->TryActivateAbilityByClass(AbilityClassToActivate);
	if (bHasBeenActivated)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClassToActivate);
		UGameplayAbility* PrimaryInstance = AbilitySpec->GetPrimaryInstance();
		if (PrimaryInstance->IsActive())
		{
			PrimaryInstance->OnGameplayAbilityEnded.AddUObject(this, &UAbilityTask_StartAbilityAndWait::OnAbilityEndedCallback);
		}
		else
		{
			OnAbilityEnded.Broadcast();
			EndTask();
		}
	}
	else
	{
		OnAbilityCannotBeActivated.Broadcast();
		EndTask();
	}
}


void UAbilityTask_StartAbilityAndWait::OnAbilityEndedCallback(UGameplayAbility* EndedAbility)
{
	OnAbilityEnded.Broadcast();
	EndTask();
}