// 

#include "GAS/Abilities/SATORI_PushAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"

USATORI_PushAbility::USATORI_PushAbility ()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_PushAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_PushAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_PushAbility: Cannot get Damage Gameplay Effect Montage ... "), *GetName());
		return;
	}

	if(!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PushAbility: Tag is not valid ... "), *GetName());
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_PushAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_PushAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_PushAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_PushAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_PushAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_PushAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_PushAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_PushAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
		int NumberOfSpheresToSpawn = (FMath::Tan(FMath::DegreesToRadians(AngleRange))) * Range * 2 / (SphereRadius * 2);
	
		//Calc for cone spawning
		FRotator RotationOfSpawn = SpawnTransform.GetRotation().Rotator();
		RotationOfSpawn.Yaw -= AngleRange / 2;
		float IncrementAngle = AngleRange / NumberOfSpheresToSpawn;

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


