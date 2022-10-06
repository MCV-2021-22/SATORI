// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_TPSpawn.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Components/Fujin/SATORI_FujinSpawns.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_TPSpawn::USATORI_TPSpawn()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_TPSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_TPSpawn::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_TPSpawn::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_TPSpawn::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_TPSpawn::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_TPSpawn::EventReceived);
	Task->ReadyForActivation();


}

void USATORI_TPSpawn::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{

		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->GetMesh()->SetVisibility(false);
			Fujin->SetActorEnableCollision(false);

		}




		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_TPSpawn::Teleport, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);

	}
}


void USATORI_TPSpawn::Teleport(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

	if (Fujin)
	{

		TArray< AActor* > Spawns;


		FName tag = "PossessedBy.Player";

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Fujin.Spawns"), Spawns);

		
		if (Spawns.Num() > 0)
		{
			int num = rand() % Spawns.Num();
			AActor* Actor = Spawns[num];
			if (Cast<ASATORI_FujinSpawns>(Actor) != nullptr)
			{
				ASATORI_FujinSpawns* Player = Cast<ASATORI_FujinSpawns>(Actor);

				FVector dest = Player->GetActorLocation();

				Fujin->SetActorLocation(dest);
				Fujin->GetMesh()->SetVisibility(true);
				Fujin->SetActorEnableCollision(true);


			}


		}

	}


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_TPSpawn::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_TPSpawn::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}