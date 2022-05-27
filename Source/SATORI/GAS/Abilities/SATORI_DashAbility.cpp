//

#include "GAS/Abilities/SATORI_DashAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"

USATORI_DashAbility::USATORI_DashAbility() 
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	bIsCreateOnRunning = GIsRunning;

}

void USATORI_DashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DashAbility: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	Player = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	if (!Player)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Cannot Cast ASATORICharacter ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//If is not moving will not Dash
	/*
	const FVector Dash = Player->GetCharacterMovement()->GetLastInputVector();
	if (!Dash.IsZero())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	*/

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Cannot Commit Ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Tag is not valid ... "), *GetName());
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DashAbility::EventReceived);
	Task->ReadyForActivation();

}

void USATORI_DashAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData) 
{
	Dashing = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData) 
{
	Dashing = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) 
{
	if (EventTag == TagEndAbility)
	{
		Dashing = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == TagSpawnAbility)
	{
		Dashing = true;
	}
}

void USATORI_DashAbility::Tick(float DeltaTime)
{
	if (Dashing)
	{
		Player->AddActorLocalOffset(Direction * DashDistance * DashSpeed * DeltaTime);
	}
}

bool USATORI_DashAbility::IsTickable() const
{
	return bIsCreateOnRunning;
}

bool USATORI_DashAbility::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_DashAbility::GetStatId() const
{
	return UObject::GetStatID();
}