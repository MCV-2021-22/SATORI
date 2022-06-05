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
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_MissileAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == TagEndAbility)
	{
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
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
		if (Character->GetTargetSystemComponent()->IsLocked())
		{
			FRotator Rotation = FRotator(0.0f, Character->GetTargetSystemComponent()->GetAngleUsingCameraRotation(Character->GetTargetSystemComponent()->GetLockedOnTargetActor()), 0.0f);
			SpawnTransform.SetLocation(Character->GetActorLocation() + Rotation.Quaternion().GetForwardVector() * 10);
			SpawnTransform.SetRotation(Rotation.Quaternion());

		}
		//Aiming when not targeting
		else
		{
			SpawnTransform.SetLocation(Character->GetActorLocation() + Character->GetActorForwardVector() * 10);
			SpawnTransform.SetRotation(Character->GetActorRotation().Quaternion());
		}

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
