// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_MisileAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Engine/Classes/Camera/CameraComponent.h"

USATORI_MisileAbility::USATORI_MisileAbility() {

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_MisileAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_MisileAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MisileAbility: Cannot Commit Ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_MisileAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_MisileAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_MisileAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_MisileAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_MisileAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_MisileAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_MisileAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_MisileAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MisileAbility: Ending Ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnAbility")))
	{

		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MisileAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		//Should work with Targeting System
		if (Character->ActorHasTag(TEXT("State.Targeting")))
		{
			UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>();
			if (!CameraComponent)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MisileAbility: Cannot Cast UCameraComponent ... "), *GetName());
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			}

			FRotator CameraRotation = CameraComponent->GetComponentRotation();
			CameraRotation.Pitch = 0.0f;
			SpawnTransform.SetLocation(Character->GetActorLocation() + CameraComponent->GetForwardVector() * 100);
			SpawnTransform.SetRotation(CameraRotation.Quaternion());

		}
		else
		{
			SpawnTransform.SetLocation(Character->GetActorLocation() + Character->GetActorForwardVector() * 100);
			SpawnTransform.SetRotation(Character->GetActorRotation().Quaternion());
		}

		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());

		ASATORI_MisileActor* Misile = GetWorld()->SpawnActorDeferred<ASATORI_MisileActor>(MisileActor, SpawnTransform, GetOwningActorFromActorInfo(),
		Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Misile->Speed = Speed;
		Misile->TimeToDestroy = TimeToDestroy;
		Misile->DamageEffectSpecHandle = DamageEffectSpecHandle;
		Misile->FinishSpawning(SpawnTransform);

	}
}
