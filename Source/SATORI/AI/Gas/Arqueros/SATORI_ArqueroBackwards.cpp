// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/SATORI_ArqueroBackwards.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_ArqueroBackwards::USATORI_ArqueroBackwards()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_ArqueroBackwards::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_ArqueroBackwards::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_ArqueroBackwards::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();

		TArray< AActor* > enemigos;


		FName tag = "PossessedBy.Player";

		if (IsClone)
		{
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character.Clone"), enemigos);

		}
		else
		{
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

		}

		for (AActor* Actor : enemigos)
		{
			if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
			{
				ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);
				bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

				UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

				FVector dest = Player->GetActorLocation();

				FRotator RotationOfIA = CurrentActorInfo->AvatarActor->GetActorRotation();
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FTransform IATransform = CurrentActorInfo->AvatarActor->GetTransform();

				ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
					CurrentActorInfo->AvatarActor->GetActorLocation() + CurrentActorInfo->AvatarActor->GetActorForwardVector() * 100,
					RotationOfIA);

				if (Sphere)
				{
					FVector newForward = dest - Sphere->GetActorLocation();
					newForward.Normalize();
					Sphere->damage = this->damage;
					Sphere->setDirection(newForward);
				}
				break;
			}
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

}


void USATORI_ArqueroBackwards::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->posinicial = Character->GetActorLocation();
		Character->moveBackwards = true;
	}
	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_ArqueroBackwards::OnTimerFinished, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);
	

}



void USATORI_ArqueroBackwards::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
		ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 100,
		RotationOfIA);
	
			//ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
	Character->moveBackwards = false;
	Character->firstmoveBackwards = true;
	

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	
}