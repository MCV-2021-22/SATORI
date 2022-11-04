//

#include "GAS/Abilities/SATORI_HitReactionAbility.h"
#include "Character/SATORI_PlayerController.h"

USATORI_HitReactionAbility::USATORI_HitReactionAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_HitReactionAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontageLightDamage) || !IsValid(AnimMontageHeavyDamage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_HitReactionAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagEndAbility.IsValid() || !TagStartAbility.IsValid() || !TagLightDamage.IsValid() || !TagHeavyDamage.IsValid() )
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_HitReactionAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_HitReactionAbility: Cannot Cast ASATORICharacter ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (Character->GetHealth() <= 0)
	{
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_HitReactionAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (Character->HasMatchingGameplayTag(TagLightDamage))
	{
		USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontageLightDamage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &USATORI_HitReactionAbility::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &USATORI_HitReactionAbility::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &USATORI_HitReactionAbility::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &USATORI_HitReactionAbility::OnCancelled);
		Task->EventReceived.AddDynamic(this, &USATORI_HitReactionAbility::EventReceived);
		Task->ReadyForActivation();
	}
	else if (Character->HasMatchingGameplayTag(TagHeavyDamage))
	{
		USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontageHeavyDamage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &USATORI_HitReactionAbility::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &USATORI_HitReactionAbility::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &USATORI_HitReactionAbility::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &USATORI_HitReactionAbility::OnCancelled);
		Task->EventReceived.AddDynamic(this, &USATORI_HitReactionAbility::EventReceived);
		Task->ReadyForActivation();
	}
}

void USATORI_HitReactionAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController()))
	{
		Character->EnableInput(Controller);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_HitReactionAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_HitReactionAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_HitReactionAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == TagStartAbility)
	{
		if (ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController()))
		{
			Character->DisableInput(Controller);
		}
	}

	if (EventTag == TagEndAbility)
	{
		if (ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController()))
		{
			Character->EnableInput(Controller);
		}
	}

}