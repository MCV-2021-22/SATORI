// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_SAT_TimeSlow.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "SATORICharacter.h"
#include "GameFramework/MovementComponent.h"

void UGA_SAT_TimeSlow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	StartTimeSlow(Handle, ActorInfo, ActivationInfo);
}

void UGA_SAT_TimeSlow::StartTimeSlow(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
	ActorInfo->AvatarActor->CustomTimeDilation = 2.0f;
	TimerDelegate = FTimerDelegate::CreateUObject(this, &UGA_SAT_TimeSlow::OnTimerFinished, Handle, ActorInfo, ActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityTime * 0.5f, false);
}

void UGA_SAT_TimeSlow::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	ActorInfo->AvatarActor->CustomTimeDilation = 1.0f;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGA_SAT_TimeSlow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
