// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_DefensePush.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_DefensePush::USATORI_DefensePush()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_DefensePush::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DefensePush::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DefensePush::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DefensePush::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DefensePush::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DefensePush::EventReceived);
	Task->ReadyForActivation();

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}





void USATORI_DefensePush::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();

		TArray< AActor* > enemigos;

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

		// Particles
		if (Area_Particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				Area_Particle, IA_POS, FRotator::ZeroRotator, true);
		}

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


				ASATORI_RaijinDefensePush* Empuje = GetWorld()->SpawnActor<ASATORI_RaijinDefensePush>(ProjectileClass,
					IA_POS,
					RotationOfIA);

				Empuje->PushPlayer(Player);


				break;

			}

		}



		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	}
}

void USATORI_DefensePush::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_DefensePush::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}