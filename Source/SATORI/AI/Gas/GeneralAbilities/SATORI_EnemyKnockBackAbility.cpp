// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/GeneralAbilities/SATORI_EnemyKnockBackAbility.h"
#include "AbilitySystemComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameplayEffect.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Actors/AbilitiesActors/SATORI_PushActor.h"

USATORI_EnemyKnockBackAbility::USATORI_EnemyKnockBackAbility()
{
	
}

void USATORI_EnemyKnockBackAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_PushAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PushAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PushAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_EnemyKnockBackAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_EnemyKnockBackAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_EnemyKnockBackAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_EnemyKnockBackAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_EnemyKnockBackAbility::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_EnemyKnockBackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_EnemyKnockBackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FTimerHandle TimerHandleEndAbility;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_EnemyKnockBackAbility::FinishWaitingForEnd, TimeToEndAbility, false);
}

void USATORI_EnemyKnockBackAbility::FinishWaitingForEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_EnemyKnockBackAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(GetOwningActorFromActorInfo());

		FTransform SpawnTransform = Character->GetTransform();
		FRotator Rotation = Character->GetActorRotation();
		Rotation.Pitch = 0.0f;
		SpawnTransform.SetRotation(Rotation.Quaternion());

		ASATORI_PushActor* EnemyPushed = GetWorld()->SpawnActorDeferred<ASATORI_PushActor>(PushActor, SpawnTransform, Character,
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		EnemyPushed->DamageGameplayEffect = DamageGameplayEffect;
		EnemyPushed->Damage = 0.0f;
		EnemyPushed->TimeToFinish = TimeToEndAbility;

		EnemyPushed->FinishSpawning(SpawnTransform);
	}
}
