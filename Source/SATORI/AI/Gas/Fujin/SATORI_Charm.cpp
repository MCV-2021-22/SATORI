// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_Charm.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_Charm::USATORI_Charm()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_Charm::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_Charm::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_Charm::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_Charm::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_Charm::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_Charm::EventReceived);
	Task->ReadyForActivation();


}

void USATORI_Charm::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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

				ASATORI_FujinCharm* Sphere = GetWorld()->SpawnActor<ASATORI_FujinCharm>(Charm,
					CurrentActorInfo->AvatarActor->GetActorLocation() + CurrentActorInfo->AvatarActor->GetActorForwardVector() * 100,
					RotationOfIA);

				if (Sphere)
				{
					FVector newForward = dest - Sphere->GetActorLocation();
					newForward.Normalize();
					Sphere->damage = this->damage;
					Sphere->setDirection(newForward * 20);

					ASATORI_Fujin* Fujin1 = Cast<ASATORI_Fujin>(CurrentActorInfo->AvatarActor);
					if (Fujin1)
					{
						Sphere->Fujin = Fujin1;
					}
					CharmDone = Sphere;
					time_alive = 4.0f;
				}
				break;
			}
		}
		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_Charm::CheckCharmDone, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.3f, true);

	}
}



void USATORI_Charm::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_Charm::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

	


void USATORI_Charm::CheckCharmDone(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	AActor* Actor = GetAvatarActorFromActorInfo();

	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Actor);
	if (Fujin)
	{
		if(time_alive<0 && !CharmDone->ReturnToFujin)
		{
			CharmDone->canDestroy = true;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		else if(CharmDone->ReturnToFujinDone)
		{
			Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Fujin.Charmhit"));
			CharmDone->canDestroy = true;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		else if(!CharmDone->ReturnToFujin)
		{
			time_alive -= 0.3f;
		}

	}

}

