// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_PunchRaf.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "Components/CapsuleComponent.h"

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

	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

	if (Fujin)
	{
		
		//Fujin->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Fujin->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


}

void USATORI_PunchRaf::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagPunchRight)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->CollisionR->SetGenerateOverlapEvents(true);
			//Fujin->CollisionR->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		

	}
	else if(EventTag == TagPunchRightEnd)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->CollisionR->SetGenerateOverlapEvents(false);
			//Fujin->CollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else if (EventTag == TagPunchLeft)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->CollisionL->SetGenerateOverlapEvents(true);
			//Fujin->CollisionL->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	else if (EventTag == TagPunchLeftEnd)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->CollisionL->SetGenerateOverlapEvents(false);
			//Fujin->CollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else if (EventTag == TagPunchHeavy)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->setGolpeFuerte(true);
			Fujin->CollisionR->SetGenerateOverlapEvents(true);
			//Fujin->CollisionR->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	else if (EventTag == TagPunchHeavyEnd)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->setGolpeFuerte(false);
			Fujin->CollisionR->SetGenerateOverlapEvents(false);
			//Fujin->CollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else if (EventTag == TagPunchHeavyL)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->setGolpeFuerte(true);
			Fujin->CollisionL->SetGenerateOverlapEvents(true);
			//Fujin->CollisionL->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	else if (EventTag == TagPunchHeavyLEnd)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->setGolpeFuerte(false);
			Fujin->CollisionL->SetGenerateOverlapEvents(false);
			//Fujin->CollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void USATORI_PunchRaf::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());
	if (Fujin)
	{
		Fujin->CollisionR->SetGenerateOverlapEvents(false);
		Fujin->CollisionL->SetGenerateOverlapEvents(false);
		Fujin->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Fujin->ActivarInputPlayer();
		//Fujin->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		//Fujin->CollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Fujin->CollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_PunchRaf::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());
	if (Fujin)
	{
		Fujin->CollisionR->SetGenerateOverlapEvents(false);
		Fujin->CollisionL->SetGenerateOverlapEvents(false);
		Fujin->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Fujin->ActivarInputPlayer();
		//Fujin->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		//Fujin->CollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Fujin->CollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}