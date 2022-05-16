// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_SAT_TimeStop.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "SATORICharacter.h"

void UGA_SAT_TimeStop::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	StartTimeStop(Handle, ActorInfo, ActivationInfo);
}

void UGA_SAT_TimeStop::StartTimeStop(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0001f);
	TimerDelegate = FTimerDelegate::CreateUObject(this, &UGA_SAT_TimeStop::OnTimerFinished, Handle, ActorInfo, ActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityTime * 0.0001f, false);
	ActorInfo->AvatarActor->CustomTimeDilation = 10000.f;
}

void UGA_SAT_TimeStop::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	ActorInfo->AvatarActor->CustomTimeDilation = 1.0f;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}


void UGA_SAT_TimeStop::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}