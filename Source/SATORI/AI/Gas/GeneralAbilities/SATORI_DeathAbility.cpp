//


#include "AI/Gas/GeneralAbilities/SATORI_DeathAbility.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "Character/SATORI_CharacterBase.h"

USATORI_DeathAbility::USATORI_DeathAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_DeathAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DashAbilityClone: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbilityClone: Cannot Cast ASATORICharacter ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->EventReceived.AddDynamic(this, &USATORI_DeathAbility::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_DeathAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	Character->Destroy();
}

void USATORI_DeathAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == TagSpawnAbility)
	{
		ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());

		//Nice To Have: 
		//Remove Abilities?
		//Remove Collisions
		//Remove All Tags
		//Lock Movement

		if (Character->GetAbilitySystemComponent()->IsValidLowLevel())
		{
			Character->GetAbilitySystemComponent()->CancelAllAbilities();
			//Character->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags();
			Character->AddGameplayTag(DeathTag);
		}

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}