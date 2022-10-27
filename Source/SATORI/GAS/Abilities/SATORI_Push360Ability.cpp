// 

#include "GAS/Abilities/SATORI_Push360Ability.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"

USATORI_Push360Ability::USATORI_Push360Ability ()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_Push360Ability::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_Push360Ability: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_Push360Ability: Cannot get Damage Gameplay Effect Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if(!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_Push360Ability: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_Push360Ability: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_Push360Ability: Cannot Cast CharacterBase ... "), *GetName());
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
	Task->OnBlendOut.AddDynamic(this, &USATORI_Push360Ability::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_Push360Ability::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_Push360Ability::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_Push360Ability::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_Push360Ability::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_Push360Ability::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
	if (Controller)
	{
		Character->EnableInput(Controller);
	}
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_Push360Ability::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
	if (Controller)
	{
		Character->EnableInput(Controller);
	}
	FTimerHandle TimerHandleEndAbility;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_Push360Ability::FinishWaitingForEnd, TimeToEndAbility, false);
}

void USATORI_Push360Ability::FinishWaitingForEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_Push360Ability::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		FTransform SpawnTransform = Character->GetTransform();

		//Push Actor creation
		ASATORI_Push360Actor* Push360 = GetWorld()->SpawnActorDeferred<ASATORI_Push360Actor>(Push360Actor, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Push360->DamageGameplayEffect = DamageGameplayEffect;
		Push360->Damage = Damage;
		Push360->TimeToFinish = TimeToEndAbility;
		Push360->FinishSpawning(SpawnTransform);
	}
}


