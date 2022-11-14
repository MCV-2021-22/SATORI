// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_AreasPatron.h"

#include "SATORICharacter.h"
#include "AI/Components/Raijin/SATORI_RaijinRayoMovilSpawns.h"
#include "Components/CapsuleComponent.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Kismet/GameplayStatics.h"

USATORI_AreasPatron::USATORI_AreasPatron()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}


void USATORI_AreasPatron::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AreasPatron::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AreasPatron::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AreasPatron::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AreasPatron::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AreasPatron::EventReceived);
	Task->ReadyForActivation();

	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
	if (Raijin)
	{
		if (Raijin->ArcoAltavoces)
		{
			Raijin->ArcoAltavoces->setAttacking(true);
			Raijin->StartPartArco();
		}
		if (Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))))
		{
			if (!Raijin->Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Raijin.C4"))))
			{
				Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Raijin.C4"));
			}
			
		}
		
	}
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}



void USATORI_AreasPatron::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
		TArray< AActor* > Spawns;

		// Particles
		if (Area_Particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				Area_Particle, IA_POS, FRotator::ZeroRotator, true);
		}

		FName tag = "PossessedBy.Player";

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Raijin.SpawnMovil"), Spawns);

		int array_dim = Spawns.Num();

		for (int i = 0; i < Raijin->getNumRayosExtensibles(); i++)
		{
			if (Spawns.Num() > 0)
			{
				int num = rand() % Spawns.Num();
				//for (AActor* Actor : Spawns)
				//{

					//Actor->Tags.Add("PossessedBy.Player");
				AActor* Actor = Spawns[num];
				if (Cast<ASATORI_RaijinRayoMovilSpawns>(Actor) != nullptr)
				{
					ASATORI_RaijinRayoMovilSpawns* Player = Cast<ASATORI_RaijinRayoMovilSpawns>(Actor);



					FVector dest = Player->GetActorLocation();

					FRotator RotationOfIA = CurrentActorInfo->AvatarActor->GetActorRotation();
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					FTransform IATransform = CurrentActorInfo->AvatarActor->GetTransform();


					ASATORI_RaijinRayoMovil* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayoMovil>(ProjectileClass,
						Player->CapsuleComponentInicio->GetRelativeLocation(),
						RotationOfIA);

					if (Rayo)
					{
						Rayo->Damage = Damage;
						FVector newForward = Player->CapsuleComponentFinal->GetRelativeLocation() - Player->CapsuleComponentInicio->GetRelativeLocation();
						newForward.Normalize();
						Rayo->setDirection(newForward);
						Rayo->CapsuleComponentFinal->SetRelativeLocation(Player->CapsuleComponentFinal->GetRelativeLocation());
						Spawns.Remove(Actor);
						Rayos.Add(Rayo);
						//Spawns.RemoveAt(num);
					}

				}
			}



			//ASATORI_ArcherProjectile* NewProjectile = World->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass, Transform, SpawnParams);
		//}
		}

		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_AreasPatron::EndRayos, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.3f, true);

	}
}

void USATORI_AreasPatron::EndRayos(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	UE_LOG(LogTemp, Warning, TEXT("Num Rayos %f"), Rayos.Num());
	if (Rayos.Num() == 0)
	{
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());

		if (Raijin)
		{
			if (Raijin->ArcoAltavoces)
			{
				Raijin->ArcoAltavoces->setAttacking(false);
				Raijin->EndPartArco();
			}
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	else
	{
		for(int j=0;j<Rayos.Num();j++)
		{
			AActor* Actor = Rayos[j];

			if(Actor)
			{
				//ASATORI_RaijinRayoMovil* Rayo = Cast<ASATORI_RaijinRayoMovil>(Actor);
				ASATORI_RaijinRayoMovil* Rayo = Cast<ASATORI_RaijinRayoMovil>(Actor);

				if(Rayo)
				{
					if (Rayo->getDestruido())
					{

						Rayos.Remove(Rayo);
						Rayo->Destroy();


					}
				}
				
			}

			

		}

	}

}

void USATORI_AreasPatron::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());

	if (Raijin)
	{
		if (Raijin->ArcoAltavoces)
		{
			Raijin->ArcoAltavoces->setAttacking(false);
			Raijin->EndPartArco();
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_AreasPatron::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
