// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/GA_ArqueroDefensivo.h"
#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "TimerManager.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "GameFramework/CharacterMovementComponent.h"




UGA_ArqueroDefensivo::UGA_ArqueroDefensivo()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UGA_ArqueroDefensivo::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_ArqueroDefensivo::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_ArqueroDefensivo::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
		if (Defensive)
		{
			Character->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));
			Character->moveBackwards = true;
		}
		TimerDelegate = FTimerDelegate::CreateUObject(this, &UGA_ArqueroDefensivo::OnTimerFinished, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, true);
	}
		
}



void UGA_ArqueroDefensivo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_ArqueroDefensivo::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_ArqueroDefensivo::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_ArqueroDefensivo::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_ArqueroDefensivo::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_ArqueroDefensivo::EventReceived);
	Task->ReadyForActivation();
	
}

void UGA_ArqueroDefensivo::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	iteracion++;
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
		ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 100,
		RotationOfIA);

	TArray< AActor* > enemigos;
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
		//Actor->Tags.Add("PossessedBy.Player");
		if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
		{
			ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			FVector dest = Player->GetActorLocation();
			if(Sphere)
			{
				FVector newForward = dest - Sphere->GetActorLocation();
				newForward.Normalize();
				Sphere->damage = this->damage;
				Sphere->setDirection(newForward );
			}
			break;
		}
	}
	
	if(iteracion == 3)
	{
		if (Defensive)
		{
			//ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
			Character->moveBackwards = false;
			Character->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));
		}

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		iteracion = 0;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		
	}
}

void UGA_ArqueroDefensivo::OnEndAb(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
	Character->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_ArqueroDefensivo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}