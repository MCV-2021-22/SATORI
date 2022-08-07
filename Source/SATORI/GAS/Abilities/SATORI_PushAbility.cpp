// 

#include "GAS/Abilities/SATORI_PushAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
//Debug
#include "DrawDebugHelpers.h"

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

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_PushAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_PushAbility: Cannot get Damage Gameplay Effect Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if(!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PushAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PushAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
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
	FTimerHandle TimerHandleEndAbility;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_PushAbility::FinishWaitingForEnd, TimeToEndAbility, false);
}

void USATORI_PushAbility::FinishWaitingForEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_PushAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_PushAbility: Cannot Cast ASATORICharacter ... "), *GetName());
			Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
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
		int QuantityToSpawn = (FMath::Tan(FMath::DegreesToRadians(AngleRange))) * Range * 2 / (BoxRadius * 2) / 2;

		//Calc for cone spawning
		FRotator RotationOfSpawn = SpawnTransform.GetRotation().Rotator();
		float IncrementAngle = AngleRange / QuantityToSpawn;
		SpawnTransform.SetRotation(RotationOfSpawn.Quaternion());

		//Push Actor creation
		for (int i = 0; i < QuantityToSpawn + 1; i++)
		{
			ASATORI_PushActor* PushR = GetWorld()->SpawnActorDeferred<ASATORI_PushActor>(PushActor, SpawnTransform, GetOwningActorFromActorInfo(),
				Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			PushR->DamageGameplayEffect = DamageGameplayEffect;
			PushR->Damage = Damage;
			PushR->TimeToFinish = TimeToEndAbility;
			PushR->FinishSpawning(SpawnTransform);

			FTransform SpawnTransformMirror = SpawnTransform;
			FRotator RotationOfSpawMirror = RotationOfSpawn;
			RotationOfSpawMirror.Yaw = RotationOfSpawMirror.Yaw - IncrementAngle * i * 2;
			SpawnTransformMirror.SetRotation(RotationOfSpawMirror.Quaternion());

			ASATORI_PushActor* PushL = GetWorld()->SpawnActorDeferred<ASATORI_PushActor>(PushActor, SpawnTransformMirror, GetOwningActorFromActorInfo(),
				Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			PushL->DamageGameplayEffect = DamageGameplayEffect;
			PushL->Damage = Damage;
			PushL->TimeToFinish = TimeToEndAbility;
			PushL->FinishSpawning(SpawnTransformMirror);

			RotationOfSpawn.Yaw += IncrementAngle;
			SpawnTransform.SetRotation(RotationOfSpawn.Quaternion());
		}
	}
}


