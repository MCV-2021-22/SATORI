// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_HeavyAttackAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "GameplayEffect.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Animation/AnimInstance.h"
#include "SATORICharacter.h"
#include "Animation/SkeletalMeshActor.h"
#include "GAS/Tasks/AbilityTask_WaitInputReleaseByTm.h"
#include "Actors/AbilitiesActors/SATORI_Push360Actor.h"

USATORI_HeavyAttackAbility::USATORI_HeavyAttackAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_HeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_HeavyAttackAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_HeavyAttackAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_HeavyAttackAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_HeavyAttackAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_HeavyAttackAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_HeavyAttackAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_HeavyAttackAbility::EventReceived);
	Task->ReadyForActivation();

	UAbilityTask_WaitInputReleaseByTm* WaitInputReleaseTask = UAbilityTask_WaitInputReleaseByTm::WaitInputReleaseByTime(this, false, MaxHoldTime);
	WaitInputReleaseTask->UpdateHoldTime();
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &USATORI_HeavyAttackAbility::OnTimerFinished);
	WaitInputReleaseTask->ReadyForActivation();
}


void USATORI_HeavyAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	FinishHoldTime = 0.0f;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_HeavyAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_HeavyAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_HeavyAttackAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		if (FinishHoldTime > KnockBackHoldTime)
		{
			ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
			if (!Character)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Cannot Cast ASATORICharacter ... "), *GetName());
				Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			}

			// Camera Shade
			Character->PlayerCameraShake();

			FTransform SpawnTransform = Character->GetTransform();

			ASATORI_Push360Actor* Push360 = GetWorld()->SpawnActorDeferred<ASATORI_Push360Actor>(Push360Actor, SpawnTransform, GetOwningActorFromActorInfo(),
				Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			Push360->DamageGameplayEffect = DamageGameplayEffect;
			Push360->Damage = 0.0f;
			Push360->TimeToFinish = TimeToEndAbility;
			Push360->FinishSpawning(SpawnTransform);
		}
	}

	FinishHoldTime = 0.0f;
}

void USATORI_HeavyAttackAbility::OnTimerFinished(float FinishTime)
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		// Attack Multiplier Section
		float CurrentAttack = Character->WeaponDamage;
		float LastAttackDamage = (((FinishTime * CurrentAttack) / 100) + AttackMultiplier) * CurrentAttack;
		Character->WeaponDamage += LastAttackDamage;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Attack power: %f"), Character->WeaponDamage));

		FinishHoldTime = FinishTime;

		// Anim Jump Section
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_JumpToSection(FName("AttackEnd"), AnimMontage);
		}
	}
}