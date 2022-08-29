// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Tasks/AbilityTask_WaitInputReleaseByTm.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h"

UAbilityTask_WaitInputReleaseByTm::UAbilityTask_WaitInputReleaseByTm(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StartTime = 0.f;
	HoldTime = 0.f;
	bTestInitialState = false;
}

void UAbilityTask_WaitInputReleaseByTm::OnReleaseByTimeCallback()
{
	if (!Ability || !AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent, IsPredictingClient());

	if (IsPredictingClient())
	{
		// Tell the server about this
		AbilitySystemComponent->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey(), AbilitySystemComponent->ScopedPredictionKey);
	}
	else
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	// We are done. Kill us so we don't keep getting broadcast messages
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnRelease.Broadcast(HoldTime);
	}

	EndTask();
}

UAbilityTask_WaitInputReleaseByTm* UAbilityTask_WaitInputReleaseByTm::WaitInputReleaseByTime(class UGameplayAbility* OwningAbility, bool bTestAlreadyReleased, float MaxHoldTime)
{
	UAbilityTask_WaitInputReleaseByTm* Task = NewAbilityTask<UAbilityTask_WaitInputReleaseByTm>(OwningAbility);
	Task->MaxHoldTime = MaxHoldTime;
	Task->bTestInitialState = bTestAlreadyReleased;
	return Task;
}

void UAbilityTask_WaitInputReleaseByTm::Activate()
{
	StartTime = GetWorld()->GetTimeSeconds();

	UpdateHoldTime();

	if (Ability)
	{
		if (bTestInitialState && IsLocallyControlled())
		{
			FGameplayAbilitySpec* Spec = Ability->GetCurrentAbilitySpec();
			if ((Spec && !Spec->InputPressed) || HoldTime >= MaxHoldTime)
			{
				OnReleaseByTimeCallback();
				return;
			}
		}

		DelegateHandle = AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UAbilityTask_WaitInputReleaseByTm::OnReleaseByTimeCallback);
		if (IsForRemoteClient())
		{
			if (!AbilitySystemComponent->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()))
			{
				SetWaitingOnRemotePlayerData();
			}
		}
	}
}

void UAbilityTask_WaitInputReleaseByTm::UpdateHoldTime()
{
	FTimerHandle TimerHandler;
	GetOwnerActor()->GetWorldTimerManager().SetTimer(TimerHandler, this, 
		&UAbilityTask_WaitInputReleaseByTm::CountMaxHoldTime, 1.0f, true, 0.0f);
}

void UAbilityTask_WaitInputReleaseByTm::CountMaxHoldTime()
{
	if (HoldTime <= MaxHoldTime)
	{
		HoldTime = HoldTime + 1;
	}

	if (HoldTime >= MaxHoldTime)
	{
		OnReleaseByTimeCallback();
		return;
	}
}