// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_PunchBasic.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_PunchBasic::USATORI_PunchBasic()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_PunchBasic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_PunchBasic::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_PunchBasic::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_PunchBasic::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_PunchBasic::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_PunchBasic::EventReceived);
	Task->ReadyForActivation();


}

void USATORI_PunchBasic::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		int a = 2;


	}
}

void USATORI_PunchBasic::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_PunchBasic::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}



