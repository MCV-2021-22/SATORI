// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_TimeStopAbility.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORICharacter.h"
#include "Kismet/GameplayStatics.h"

USATORI_TimeStop::USATORI_TimeStop()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_TimeStop::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TimeStopAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TimeStopAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TimeStopAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnInterrupted.AddDynamic(this, &USATORI_TimeStop::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_TimeStop::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_TimeStop::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_TimeStop::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_TimeStop::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		StartTimeStop();

		AActor* Player = GetAvatarActorFromActorInfo();

		if (P_Particle && Player)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Particle, Player->GetActorLocation(), FRotator::ZeroRotator, true);
		}
	}
}

void USATORI_TimeStop::StartTimeStop()
{
	GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstanceRef->TimeStop = true;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);

	TimerD = FTimerDelegate::CreateUObject(this, &USATORI_TimeStop::SetActorVelocity, CurrentActorInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerH, TimerD,0.01f, false);

	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_TimeStop::OnTimerFinished, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityTime * 0.2f, false);
	
}

void USATORI_TimeStop::SetActorVelocity(const FGameplayAbilityActorInfo* ActorInfo)
{
	ActorInfo->AvatarActor->CustomTimeDilation = 5.0f;
	GetWorld()->GetTimerManager().ClearTimer(TimerH);
}

void USATORI_TimeStop::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GameInstanceRef->TimeStop = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	CurrentActorInfo->AvatarActor->CustomTimeDilation = 1.f;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void USATORI_TimeStop::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}