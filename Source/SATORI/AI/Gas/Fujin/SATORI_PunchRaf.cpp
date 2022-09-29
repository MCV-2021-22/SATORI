// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_PunchRaf.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_PunchRaf::USATORI_PunchRaf()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_PunchRaf::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_PunchRaf::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_PunchRaf::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_PunchRaf::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_PunchRaf::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_PunchRaf::EventReceived);
	Task->ReadyForActivation();


}

void USATORI_PunchRaf::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		int a = 2;
		

	}
}

void USATORI_PunchRaf::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_PunchRaf::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}