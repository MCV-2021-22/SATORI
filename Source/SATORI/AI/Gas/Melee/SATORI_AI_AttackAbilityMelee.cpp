//

#include "AI/Gas/Melee/SATORI_AI_AttackAbilityMelee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

USATORI_AI_AttackAbilityMelee::USATORI_AI_AttackAbilityMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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
	if (Enemy)
	{
		FRotator MeleeRotation = (Enemy->GetActorLocation() - Melee->GetActorLocation()).ToOrientationRotator();
		MeleeRotation.Pitch = 0;
		Melee->SetActorRotation(MeleeRotation);
	}

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