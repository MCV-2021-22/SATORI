//

#include "GAS/Abilities/SATORI_BlackHoleAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "TimerManager.h"

USATORI_BlackHoleAbility::USATORI_BlackHoleAbility() 
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_BlackHoleAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_BlackHoleAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_BlackHoleAbility: Cannot get Damage Gameplay Effect Montage ... "), *GetName());
		return;
	}

	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_BlackHoleAbility: Tag is not valid ... "), *GetName());
	}

	TimeToEndAbility = TimeToStopGrowing + TimeToStopAttraction;

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_BlackHoleAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_BlackHoleAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_BlackHoleAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_BlackHoleAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_BlackHoleAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_BlackHoleAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_BlackHoleAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_BlackHoleAbility::FinishWaitingForEnd, TimeToEndAbility, false);
}

void USATORI_BlackHoleAbility::FinishWaitingForEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_BlackHoleAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	if (EventTag == TagEndAbility)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_BlackHoleAbility::FinishWaitingForEnd, TimeToEndAbility, false);
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

		SpawnTransform.SetRotation(Rotation.Quaternion());

		//BlackHole Actor creation
		ASATORI_BlackHoleActor* BlackHole = GetWorld()->SpawnActorDeferred<ASATORI_BlackHoleActor>(BlackHoleActor, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		BlackHole->DamageGameplayEffect = DamageGameplayEffect;
		BlackHole->Damage = Damage;
		BlackHole->TimeToStopGrowing = TimeToStopGrowing;
		BlackHole->TimeToStopAttraction = TimeToStopAttraction;
		BlackHole->FinishSpawning(SpawnTransform);
	}
}
