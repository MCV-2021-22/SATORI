//

#include "GAS/Abilities/SATORI_DecoyAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Engine/Classes/Camera/CameraComponent.h"

USATORI_DecoyAbility::USATORI_DecoyAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_DecoyAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DecoyAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DecoyAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DecoyAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DecoyAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DecoyAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DecoyAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DecoyAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DecoyAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_DecoyAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_DecoyAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FTimerHandle TimerHandleEndAbility;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_DecoyAbility::FinishWaitingForEnd, TimeToEndAbility, false);
}

void USATORI_DecoyAbility::FinishWaitingForEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DecoyAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DecoyAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FTransform SpawnTransform = Character->HandComponent->GetComponentTransform();
		FRotator Rotation;
		Rotation = Character->GetActorRotation();
		Rotation.Pitch = 0.0f;
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//Decoy Actor creation
		ASATORI_DecoyActor* Decoy = GetWorld()->SpawnActorDeferred<ASATORI_DecoyActor>(DecoyActor, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Decoy->TimeToFinish = TimeToEndAbility;
		Decoy->FinishSpawning(SpawnTransform);
	}
}

