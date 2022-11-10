// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_LightAttackAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"

void USATORI_LightAttackAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_LightAttackAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	PlayerCharacter = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbiUSATORI_LightAttackAbilitylity: Cannot Cast ASATORICharacter ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_LightAttackAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_LightAttackAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_LightAttackAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_LightAttackAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_LightAttackAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_LightAttackAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_LightAttackAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_LightAttackAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	//if (PlayerCharacter)
	//{
	//	USATORI_AbilitySystemComponent* AbilitySystemComponent =
	//		Cast<USATORI_AbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
	//	if (AbilitySystemComponent)
	//	{
	//		AbilitySystemComponent->CurrentMontageStop();
	//	}
	//}

	//UAnimInstance* AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
	//if (AnimInstance && AnimMontage)
	//{
	//	FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(AnimMontage);
	//	if (MontageInstance)
	//	{
	//		MontageInstance->OnMontageBlendingOutStarted.Unbind();
	//		MontageInstance->OnMontageEnded.Unbind();
	//		FAlphaBlend* AlfaBlend = nullptr;
	//		MontageInstance->Stop(*AlfaBlend);
	//	}
	//}

	//// This dont work good no way omg
	////PlayerCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 1.0f), false, false);

	//SourceBlockedTags.Reset();

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void USATORI_LightAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_LightAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_LightAttackAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		// Camera Shade
		Character->PlayerCameraShake();
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}