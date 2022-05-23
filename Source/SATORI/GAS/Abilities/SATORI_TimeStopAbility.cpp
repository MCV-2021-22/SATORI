// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_TimeStopAbility.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "SATORICharacter.h"

void USATORI_TimeStop::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	StartTimeStop(Handle, ActorInfo, ActivationInfo);
}

void USATORI_TimeStop::StartTimeStop(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0001f);

	TimerD = FTimerDelegate::CreateUObject(this, &USATORI_TimeStop::SetActorVelocity, ActorInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerH, TimerD,0.000001f, false);

	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_TimeStop::OnTimerFinished, Handle, ActorInfo, ActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityTime * 0.0001f, false);
	
}

void USATORI_TimeStop::SetActorVelocity(const FGameplayAbilityActorInfo* ActorInfo)
{
	ActorInfo->AvatarActor->CustomTimeDilation = 10000.f;
}


void USATORI_TimeStop::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	ActorInfo->AvatarActor->CustomTimeDilation = 1.0f;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}


void USATORI_TimeStop::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}