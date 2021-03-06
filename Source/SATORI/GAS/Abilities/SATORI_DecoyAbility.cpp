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
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DecoyAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DecoyAbility: Cannot get Damage Gameplay Effect Montage ... "), *GetName());
		return;
	}

	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DecoyAbility: Tag is not valid ... "), *GetName());
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
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DecoyAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == TagEndAbility)
	{
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == TagSpawnAbility)
	{

		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DecoyAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FTransform SpawnTransform = Character->HandComponent->GetComponentTransform();
		FRotator Rotation;

		//Aiming when Targeting Enemy
		if (Character->GetTargetSystemComponent()->IsLocked())
		{
			FVector Target = Character->GetTargetSystemComponent()->GetLockedOnTargetActor()->GetActorLocation();
			FVector Position = GetAvatarActorFromActorInfo()->GetActorLocation();
			Rotation = FRotationMatrix::MakeFromX(Target - Position).Rotator();
		}
		//Aiming when not targeting
		else
		{
			Rotation = Character->GetActorRotation();
		}

		Rotation.Pitch = 0.0f;
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//Decoy Actor creation
		ASATORI_DecoyActor* Decoy = GetWorld()->SpawnActorDeferred<ASATORI_DecoyActor>(DecoyActor, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Decoy->DamageGameplayEffect = DamageGameplayEffect;
		Decoy->Damage = Damage;
		Decoy->TimeToDestroy = TimeToDestroy;
		Decoy->FinishSpawning(SpawnTransform);
	}
}

