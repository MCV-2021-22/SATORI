// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_DmgArea.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_DmgArea::USATORI_DmgArea()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_DmgArea::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DmgArea::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DmgArea::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DmgArea::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DmgArea::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DmgArea::EventReceived);
	Task->ReadyForActivation();


}

void USATORI_DmgArea::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		TArray< AActor* > enemigos;

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

		for (AActor* Actor : enemigos)
		{

			//Actor->Tags.Add("PossessedBy.Player");
			if (Cast<ASATORICharacter>(Actor) != nullptr)
			{
				ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
				bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

				UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

				FVector dest = Player->GetActorLocation();

				FRotator RotationOfIA = CurrentActorInfo->AvatarActor->GetActorRotation();
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FTransform IATransform = CurrentActorInfo->AvatarActor->GetTransform();


				ASATORI_FujinKickAereo* Rayo = GetWorld()->SpawnActor<ASATORI_FujinKickAereo>(ProjectileClass,
					Player->GetActorLocation(),
					RotationOfIA);


				break;

			}

		}
		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_DmgArea::CheckFujin, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.3f, true);

	}
}

void USATORI_DmgArea::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_DmgArea::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_DmgArea::CheckFujin(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	AActor* Actor = GetAvatarActorFromActorInfo();

	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Actor);
	if(Fujin)
	{
		if(Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Fujin.KickDone"))))
		{
			if(USkeletalMeshComponent* Mesh = Fujin->GetMesh())
			{
				if(UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
				{
					AnimInstance->Montage_JumpToSection(FName("2"), AnimInstance->GetCurrentActiveMontage());
				}
			}


		}
	}

}

