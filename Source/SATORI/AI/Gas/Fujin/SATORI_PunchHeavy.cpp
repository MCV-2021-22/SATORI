// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_PunchHeavy.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_PunchHeavy::USATORI_PunchHeavy()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_PunchHeavy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_PunchHeavy::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_PunchHeavy::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_PunchHeavy::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_PunchHeavy::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_PunchHeavy::EventReceived);
	Task->ReadyForActivation();


}

void USATORI_PunchHeavy::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		int a = 2;


	}
}

void USATORI_PunchHeavy::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_PunchHeavy::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


