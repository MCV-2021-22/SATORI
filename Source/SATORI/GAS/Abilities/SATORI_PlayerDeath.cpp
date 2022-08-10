// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_PlayerDeath.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Character/SATORI_CharacterBase.h"
#include "Character/SATORI_PlayerController.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "GameFramework/CharacterMovementComponent.h"

void USATORI_PlayerDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Character = Cast<ASATORI_CharacterBase>(GetOwningSatoriCharacter());

	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PlayerDeath: Cannot Cast ASATORICharacter ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
	if (Controller)
	{
		Character->DisableInput(Controller);
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_PlayerDeath::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_PlayerDeath::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_PlayerDeath::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_PlayerDeath::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_PlayerDeath::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_PlayerDeath::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{

}

void USATORI_PlayerDeath::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_PlayerDeath::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_PlayerDeath::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}