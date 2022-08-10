//

#include "GAS/Abilities/SATORI_CloneAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Engine/Classes/Camera/CameraComponent.h"

USATORI_CloneAbility::USATORI_CloneAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_CloneAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_CloneAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_CloneAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_CloneAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_CloneAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_CloneAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_CloneAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_CloneAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_CloneAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FTimerHandle TimerHandleEndAbility;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_CloneAbility::FinishWaitingForEnd, TimeToEndAbility, false);
}

void USATORI_CloneAbility::FinishWaitingForEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_CloneAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		//No need to aim it
		FTransform SpawnTransform = Character->HandComponent->GetComponentTransform();
		FRotator Rotation = Character->GetActorRotation();

		SpawnTransform.SetRotation(Rotation.Quaternion());
		SpawnTransform.SetTranslation(SpawnTransform.GetLocation() + Character->GetActorForwardVector() * 50);

		//Clone creation
		ASATORI_CloneCharacter* Clone = GetWorld()->SpawnActorDeferred<ASATORI_CloneCharacter>(CloneCharacter, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Clone->FinishSpawning(SpawnTransform);
		Clone->SpawnDefaultController();
	}
}


