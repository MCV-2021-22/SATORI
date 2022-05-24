//

#include "GAS/Abilities/SATORI_MissileAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Engine/Classes/Camera/CameraComponent.h"

USATORI_MissileAbility::USATORI_MissileAbility() 
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_MissileAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_MissileAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MissileAbility: Cannot Commit Ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid() || !PlayerTargetingTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MissileAbility: Tag is not valid ... "), *GetName());
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_MissileAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_MissileAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_MissileAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_MissileAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_MissileAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_MissileAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_MissileAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_MissileAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == TagEndAbility)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == TagSpawnAbility)
	{

		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MissileAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		//Aiming when Targeting Enemy
		if (Character->ActorHasTag(PlayerTargetingTag.GetTagName()))
		{
			UCameraComponent* CameraComponent = Character->FindComponentByClass<UCameraComponent>();
			if (!CameraComponent)
			{
				UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MissileAbility: Cannot Cast UCameraComponent ... "), *GetName());
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

		//Missile Actor creation
		ASATORI_MissileActor* Missile = GetWorld()->SpawnActorDeferred<ASATORI_MissileActor>(MissileActor, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Missile->Speed = Speed;
		Missile->TimeToDestroy = TimeToDestroy;
		Missile->FinishSpawning(SpawnTransform);

	}
}
