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
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Cannot Commit Ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
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
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_CloneAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_CloneAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == FGameplayTag::RequestGameplayTag(TagEndAbility))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == FGameplayTag::RequestGameplayTag(TagSpawnAbility))
	{

		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		//Aiming when Targeting Enemy
		if (Character->ActorHasTag(PlayerTargetingTag))
		{
			UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>();
			if (!CameraComponent)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_CloneAbility: Cannot Cast UCameraComponent ... "), *GetName());
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			}

			FRotator CameraRotation = CameraComponent->GetComponentRotation();
			CameraRotation.Pitch = 0.0f;
			SpawnTransform.SetLocation(Character->GetActorLocation() + CameraComponent->GetForwardVector() * 100);
			SpawnTransform.SetRotation(CameraRotation.Quaternion());

		}
		//Aiming when not targeting
		else
		{
			SpawnTransform.SetLocation(Character->GetActorLocation() + Character->GetActorForwardVector() * 100);
			SpawnTransform.SetRotation(Character->GetActorRotation().Quaternion());
		}

		//Clone Actor creation
		//ASATORI_CloneActor* Clone = GetWorld()->SpawnActorDeferred<ASATORI_CloneActor>(CloneActor, SpawnTransform, GetOwningActorFromActorInfo(),
		//	Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//Clone->SphereRadiusOfAction = SphereRadiusOfAction;
		//Clone->TimeToDestroy = TimeToDestroy;
		//Clone->TagGrantedWhenLured = TagGrantedWhenLured;
		//Clone->FinishSpawning(SpawnTransform);

	}
}


