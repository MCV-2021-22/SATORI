//

#include "GAS/Abilities/SATORI_TornadoAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "TimerManager.h"

USATORI_TornadoAbility::USATORI_TornadoAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_TornadoAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TornadoAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TornadoAbility: Cannot get Damage Gameplay Effect ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TornadoAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TornadoAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_TornadoAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_TornadoAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_TornadoAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_TornadoAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_TornadoAbility::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_TornadoAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_TornadoAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FTimerHandle TimerHandleEndAbility;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleEndAbility, this, &USATORI_TornadoAbility::FinishWaitingForEnd, TimeToEndAbility, false);
}

void USATORI_TornadoAbility::FinishWaitingForEnd()
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_TornadoAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TornadoAbility: Cannot Cast ASATORICharacter ... "), *GetName());
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

		//Fix for not bouncing of stairs  when spawning
		FVector HeightFixForStairs = SpawnTransform.GetLocation();
		HeightFixForStairs.Z += 100.0f;
		SpawnTransform.SetLocation(HeightFixForStairs);

		SpawnTransform.SetRotation(Rotation.Quaternion());

		//Tornado Actor creation
		ASATORI_TornadoActor* Tornado = GetWorld()->SpawnActorDeferred<ASATORI_TornadoActor>(TornadoActor, SpawnTransform, GetOwningActorFromActorInfo(),
			Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Tornado->DamageGameplayEffect = DamageGameplayEffect;
		Tornado->Damage = Damage;
		Tornado->TimeToFinish = TimeToEndAbility;
		Tornado->FinishSpawning(SpawnTransform);
	}
}
