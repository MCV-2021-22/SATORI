// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/GA_ArquerosMisil.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

UGA_ArquerosMisil::UGA_ArquerosMisil()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ArquerosMisil::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_ArquerosMisil::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_ArquerosMisil::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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


void UGA_ArquerosMisil::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_ArquerosMisil::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_ArquerosMisil::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_ArquerosMisil::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_ArquerosMisil::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_ArquerosMisil::EventReceived);
	Task->ReadyForActivation();
}



