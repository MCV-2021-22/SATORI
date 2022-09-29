// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_Rayo.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_Rayo::USATORI_Rayo()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}


void USATORI_Rayo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_Rayo::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_Rayo::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_Rayo::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_Rayo::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_Rayo::EventReceived);
	Task->ReadyForActivation();
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
	if (Raijin)
	{
		Raijin->ArcoAltavoces->setAttacking(true);
	}

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void USATORI_Rayo::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_Rayo::OnBucleRayos, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, true);

	}
}


void USATORI_Rayo::OnBucleRayos(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	iteracion++;

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

			FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform IATransform = ActorInfo->AvatarActor->GetTransform();


			ASATORI_RaijinRayo* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayo>(ProjectileClass,
				Player->GetActorLocation(),
				RotationOfIA);


			break;

		}

	}


	if (iteracion == max_iteracion)
	{
		
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		iteracion = 0;
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
		if (Raijin)
		{
			Raijin->ArcoAltavoces->setAttacking(false);
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	}


}




void USATORI_Rayo::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
	if (Raijin)
	{
		Raijin->ArcoAltavoces->setAttacking(false);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_Rayo::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}





