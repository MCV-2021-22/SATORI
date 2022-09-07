// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/SATORI_ArqueroRotate.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_ArqueroRotate::USATORI_ArqueroRotate()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}




void USATORI_ArqueroRotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	/*USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_ArqueroBackwards::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_ArqueroBackwards::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_ArqueroBackwards::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_ArqueroBackwards::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_ArqueroBackwards::EventReceived);
	Task->ReadyForActivation();*/
	int side = rand() % 2;

	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		if (Character->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockRight")))
		{
			Character->moveLeft = true;
		}
		else if(Character->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockLeft")))
		{
			Character->moveRight = true;
		}
		else
		{
			if (side == 0)
			{
				Character->moveLeft = true;
			}
			else
			{
				Character->moveRight = true;
			}
		}
			
	}
	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_ArqueroRotate::OnTimerFinished, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.3f, true);


}



void USATORI_ArqueroRotate::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());

	if(iteracion < max_iteracion)
	{
		

		//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		
		Character->moveLeft = false;
		Character->moveRight = false;


		int side = rand() % 2;
		if (side == 0)
		{
			Character->moveLeft = true;
		}
		else
		{
			Character->moveRight = true;
		}
		iteracion++;


	}
	else
	{
		Character->moveLeft = false;
		Character->moveRight = false;

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	



	


}