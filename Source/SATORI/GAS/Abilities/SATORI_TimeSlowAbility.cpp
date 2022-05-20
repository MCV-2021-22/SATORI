// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_TimeSlowAbility.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void USATORI_TimeSlowAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	StartTimeSlow();
}

void USATORI_TimeSlowAbility::StartTimeSlow()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USATORI_TimeSlowAbility::OnTimerFinished, AbilityTime, false);
}

void USATORI_TimeSlowAbility::OnTimerFinished()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void USATORI_TimeSlowAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
