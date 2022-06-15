//

#include "AI/Gas/Clone/SATORI_DashAbilityClone.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


USATORI_DashAbilityClone::USATORI_DashAbilityClone()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bIsCreateOnRunning = GIsRunning;
}

void USATORI_DashAbilityClone::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DashAbilityClone: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	Clone = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Clone)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbilityClone: Cannot Cast ASATORICharacter ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbilityClone::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DashAbilityClone::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbilityClone::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DashAbilityClone::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DashAbilityClone::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_DashAbilityClone::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_DashAbilityClone::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbilityClone::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbilityClone::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagEndAbility)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == TagSpawnAbility)
	{
		bDashing = true;
	}
}

void USATORI_DashAbilityClone::Tick(float DeltaTime)
{
	if(bDashing)
	{
		FVector Position = Clone->GetActorLocation();

		FVector Forward = Clone->GetActorForwardVector();

		Clone->SetActorLocation(Position + Forward * DashDistance * DeltaTime);
	}
}

bool USATORI_DashAbilityClone::IsTickable() const
{
	return IsActive();
}

bool USATORI_DashAbilityClone::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_DashAbilityClone::GetStatId() const
{
	return UObject::GetStatID();
}