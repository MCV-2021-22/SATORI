// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_PunchLevantar.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_PunchLevantar::USATORI_PunchLevantar()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_PunchLevantar::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_PunchLevantar::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_PunchLevantar::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_PunchLevantar::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_PunchLevantar::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_PunchLevantar::EventReceived);
	Task->ReadyForActivation();


}

void USATORI_PunchLevantar::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		int a = 2;

		FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();

		TArray< AActor* > enemigos;

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

		for (AActor* Actor : enemigos)
		{

			//Actor->Tags.Add("PossessedBy.Player");
			if (Cast<ASATORICharacter>(Actor) != nullptr)
			{
				ASATORICharacter* Player1 = Cast<ASATORICharacter>(Actor);
				bool tiene = Player1->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

				FVector dest = Player1->GetActorLocation();

				FRotator RotationOfIA = CurrentActorInfo->AvatarActor->GetActorRotation();
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FTransform IATransform = CurrentActorInfo->AvatarActor->GetTransform();


				ASATORI_FujinLevantarCollision* Empuje = GetWorld()->SpawnActor<ASATORI_FujinLevantarCollision>(ProjectileClass,
					dest,
					RotationOfIA);

				Empuje->ApplyDmg(Player1);

				Empuje->PushPlayer(Player1);



				Player = Player1;

				break;

			}

		}
		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_PunchLevantar::checkCollisionPlayer, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, true);



	}
}



void USATORI_PunchLevantar::checkCollisionPlayer(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	if (Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.EndLevantado")))
	{
		Player->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.EndLevantado"));
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		Player->EnableInput(PlayerController);

		AActor* Actor = GetAvatarActorFromActorInfo();

		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Actor);
		if(Fujin)
		{
			Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Fujin.Charmhit"));
		}
		
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	
}


void USATORI_PunchLevantar::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_PunchLevantar::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}