//

#include "GAS/Abilities/SATORI_DashAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"

USATORI_DashAbility::USATORI_DashAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bIsCreateOnRunning = GIsRunning;

}

void USATORI_DashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DashAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Cannot Cast ASATORICharacter ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
	if (Controller)
	{
		Character->DisableInput(Controller);
	}

	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Tag is not valid ... "), *GetName());
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DashAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_DashAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Character = Cast<ASATORI_CharacterBase>(GetOwningSatoriCharacter());
	if(Character)
	{
		ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
		if (Controller)
		{
			Character->EnableInput(Controller);
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_DashAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagEndAbility)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}

void USATORI_DashAbility::Tick(float DeltaTime)
{
	Character->AddActorLocalOffset(Direction * DashDistance * DashSpeed * DeltaTime);
}

bool USATORI_DashAbility::IsTickable() const
{
	return IsActive();
}

bool USATORI_DashAbility::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_DashAbility::GetStatId() const
{
	return UObject::GetStatID();
}