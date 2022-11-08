//

#include "AI/Gas/Melee/SATORI_AI_AttackAbilityMelee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

USATORI_AI_AttackAbilityMelee::USATORI_AI_AttackAbilityMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsCreateOnRunning = GIsRunning;
}

void USATORI_AI_AttackAbilityMelee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_AI_AttackAbilityMelee: Cannot get Animation Montage ... "), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Melee = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	if (!Melee)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_AI_AttackAbilityMelee: Cannot Cast Character ... "), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_AI_AttackAbilityMelee: Failed commit ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	GetTarget();

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AI_AttackAbilityMelee::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AI_AttackAbilityMelee::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AI_AttackAbilityMelee::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AI_AttackAbilityMelee::OnCancelled);
	Task->ReadyForActivation();
}

void USATORI_AI_AttackAbilityMelee::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_AI_AttackAbilityMelee::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_AI_AttackAbilityMelee::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//Targets Player/Clone
void USATORI_AI_AttackAbilityMelee::GetTarget()
{
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetAvatarActorFromActorInfo());

	if (IsValid(Blackboard))
	{
		FName Clone = "Clone";
		FName Player = "Target";

		ASATORI_CharacterBase* Target = nullptr;
		Target = Cast<ASATORI_CharacterBase>(Blackboard->GetValueAsObject(Clone));

		//Check clone
		if (IsValid(Target))
		{
			Enemy = Target;
		}
		//If clone fails then checks for player
		else
		{
			Target = Cast<ASATORI_CharacterBase>(Blackboard->GetValueAsObject(Player));

			if (IsValid(Target))
			{
				Enemy = Target;
			}
		}
	}
}

void USATORI_AI_AttackAbilityMelee::RotateToTargetGradual(float DeltaTime)
{
	FRotator Rotation = (Enemy->GetActorLocation() - Melee->GetActorLocation()).Rotation();
	FVector RotationDirection = Rotation.UnrotateVector(Melee->GetActorForwardVector());
	FRotator RotationChange = RotationDirection.Rotation();
	RotationChange.Yaw = FMath::Clamp(RotationChange.Yaw, -MaxRotationChange, MaxRotationChange);
	RotationChange.Pitch = 0;
	Melee->SetActorRotation(Melee->GetActorRotation() - RotationChange);
}

void USATORI_AI_AttackAbilityMelee::RotateToTargetSnap()
{
	FRotator Rotation = (Enemy->GetActorLocation() - Melee->GetActorLocation()).Rotation();
	Rotation.Pitch = 0;
	Melee->SetActorRotation(Rotation);
}

void USATORI_AI_AttackAbilityMelee::MovementToTarget(float DeltaTime)
{
	if(Enemy && Melee)
	{
		float Distance = FVector::Dist(Enemy->GetActorLocation(), Melee->GetActorLocation());
		if (Distance > Melee->GetAttackDistance())
		{
			Melee->SetActorLocation(Melee->GetActorLocation() + Melee->GetActorForwardVector() * FMath::Clamp(Distance - Melee->GetAttackDistance(), 0.0f, ClampDistanceAttack) * MovementSpeed * DeltaTime);
		}
	}
}

void USATORI_AI_AttackAbilityMelee::Tick(float DeltaTime)
{

	if (!IsValid(Melee))
	{
		return;
	}

	if (!Melee->IsPendingKill()) {
		//Targets smoothly player
		if (Melee->HasMatchingGameplayTag(TargetingTag))
		{
			RotateToTargetGradual(DeltaTime);
		}

		//Targets Fast once and snapy, also moves
		if (Melee->HasMatchingGameplayTag(CanMoveTag))
		{
			if (!RotationChanged)
			{
				RotateToTargetSnap();
				RotationChanged = true;
			}

			MovementToTarget(DeltaTime);
		}
		else
		{
			RotationChanged = false;
		}
	}
}

bool USATORI_AI_AttackAbilityMelee::IsTickable() const
{
	return IsActive();
}

bool USATORI_AI_AttackAbilityMelee::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_AI_AttackAbilityMelee::GetStatId() const
{
	return UObject::GetStatID();
}