// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_TimeSlowAbility.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORICharacter.h"
#include "Kismet/GameplayStatics.h"

USATORI_TimeSlow::USATORI_TimeSlow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_TimeSlow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TimeSlowAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TimeSlowAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TimeSlowAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnInterrupted.AddDynamic(this, &USATORI_TimeSlow::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_TimeSlow::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_TimeSlow::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_TimeSlow::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_TimeSlow::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		StartTimeSlow();

		AActor* Player = GetAvatarActorFromActorInfo();

		if (P_Particle && Player)
		{
			FVector NewLocationVector(Player->GetActorLocation().X, Player->GetActorLocation().Y - Subtract_Particle_Loc_Num,
				Player->GetActorLocation().Z);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Particle, NewLocationVector, FRotator::ZeroRotator, true);
		}
	}
}

void USATORI_TimeSlow::StartTimeSlow()
{
	GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstanceRef->TimeSlow = true;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
	CurrentActorInfo->AvatarActor->CustomTimeDilation = 2.0f;
	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_TimeSlow::OnTimerFinished, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityTime * 0.5f, false);
}

void USATORI_TimeSlow::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GameInstanceRef->TimeSlow = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	ActorInfo->AvatarActor->CustomTimeDilation = 1.0f;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void USATORI_TimeSlow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}