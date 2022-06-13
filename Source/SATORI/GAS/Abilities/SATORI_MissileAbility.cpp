//

#include "GAS/Abilities/SATORI_MissileAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"

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
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_MissileAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_MissileAbility: Cannot get Damage Gameplay Effect Montage ... "), *GetName());
		return;
	}
	
	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
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
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_MissileAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_MissileAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_MissileAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		//No need to aim it
		FTransform SpawnTransform = Character->HandComponent->GetComponentTransform();
		FRotator Rotation = SpawnTransform.GetRotation().Rotator();

		Rotation.Pitch = 0.0f;
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//Missile Actor creation
		ASATORI_MissileActor* Missile = GetWorld()->SpawnActorDeferred<ASATORI_MissileActor>(MissileActor, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Missile->DamageGameplayEffect = DamageGameplayEffect;
		Missile->Damage = Damage;
		Missile->Range = Range;
		Missile->TimeToDestroy = TimeToDestroy;
		Missile->FinishSpawning(SpawnTransform);
	}
}
