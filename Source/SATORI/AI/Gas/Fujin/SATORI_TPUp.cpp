// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_TPUp.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "NiagaraFunctionLibrary.h"

USATORI_TPUp::USATORI_TPUp()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_TPUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_TPUp::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_TPUp::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_TPUp::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_TPUp::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_TPUp::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_TPUp::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

		if (Fujin)
		{
			Fujin->GetMesh()->SetVisibility(false);
			Fujin->SetActorEnableCollision(false);
			
			// TP Particle
			FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

			if (Teleport_Particle)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Teleport_Particle, SpawnLocation);
			}
		}

		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), EventLaugh, GetAvatarActorFromActorInfo()->GetActorTransform(), true);

		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_TPUp::Teleport, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);
	}
}


void USATORI_TPUp::Teleport(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(GetAvatarActorFromActorInfo());

	if (Fujin)
	{
		TArray< AActor* > enemigos;

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

		for (AActor* Actor : enemigos)
		{
			//Actor->Tags.Add("PossessedBy.Player");
			if (Cast<ASATORICharacter>(Actor) != nullptr)
			{
				ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
				Fujin->SetActorLocation(Player->GetActorLocation() + FVector(0,0,1000));
				Fujin->GetMesh()->SetVisibility(true);
				Fujin->SetActorEnableCollision(true);
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_TPUp::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_TPUp::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}