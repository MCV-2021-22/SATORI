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
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_Push360Ability: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_Push360Ability: Cannot get Damage Gameplay Effect Montage ... "), *GetName());
		return;
	}

	if(!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_Push360Ability: Tag is not valid ... "), *GetName());
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
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_Push360Ability::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_Push360Ability::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PushAbility: Cannot Cast ASATORICharacter ... "), *GetName());
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

		//This calcs are for designing parameters for the ability
		// 
		//Calc for number of spheres to spawn to cover all range
		int NumberOfSpheresToSpawn = (3.14 *  Range * 2) / (SphereRadius / 2);

		//Calc for cone spawning
		FRotator RotationOfSpawn = SpawnTransform.GetRotation().Rotator();
		float IncrementAngle = 360 * 2 / NumberOfSpheresToSpawn;
		RotationOfSpawn.Yaw = 0;

		SpawnTransform.SetRotation(RotationOfSpawn.Quaternion());

		//Push Actor creation
		for (int i = 0; i < NumberOfSpheresToSpawn + 1; i++)
		{
			ASATORI_PushActor* Push = GetWorld()->SpawnActorDeferred<ASATORI_PushActor>(PushActor, SpawnTransform, GetOwningActorFromActorInfo(),
				Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			Push->DamageGameplayEffect = DamageGameplayEffect;
			Push->Damage = Damage;
			Push->Speed = Speed;
			Push->PushForce = PushForce;
			Push->TimeToDestroy = TimeToDestroy;
			Push->FinishSpawning(SpawnTransform);

			RotationOfSpawn.Yaw += IncrementAngle;
			SpawnTransform.SetRotation(RotationOfSpawn.Quaternion());
		}
	}
}


