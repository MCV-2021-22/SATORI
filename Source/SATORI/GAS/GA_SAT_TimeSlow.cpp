// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_SAT_TimeSlow.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void UGA_SAT_TimeSlow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	StartTimeSlow();
}

void UGA_SAT_TimeSlow::StartTimeSlow()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGA_SAT_TimeSlow::OnTimerFinished, AbilityTime, false); 
}

void UGA_SAT_TimeSlow::OnTimerFinished()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UGA_SAT_TimeSlow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
