// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_AreaExtensible.h"

#include "SATORICharacter.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

USATORI_AreaExtensible::USATORI_AreaExtensible()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	

}

void USATORI_AreaExtensible::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AreaExtensible::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AreaExtensible::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AreaExtensible::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AreaExtensible::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AreaExtensible::EventReceived);
	Task->ReadyForActivation();


	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
	if(Raijin)
	{
		if(Raijin->ArcoAltavoces)
		{
			Raijin->ArcoAltavoces->setAttacking(true);
			//Raijin->StartPartArco();
		}
		
		

		if (Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))))
		{
			if (!Raijin->Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Raijin.C3"))))
			{
				Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Raijin.C3"));
			}
			
		}
		
	}
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}



void USATORI_AreaExtensible::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();

		TArray< AActor* > enemigos;

		FName tag = "PossessedBy.Player";

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

		// Particles
		if (Area_Particle)
		{
			Particula_loop= UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				Area_Particle, IA_POS, FRotator::ZeroRotator, true);

			
		}

		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORICharacter::StaticClass(), enemigos);

		for (AActor* Actor : enemigos)
		{
			//Actor->Tags.Add("PossessedBy.Player");
			if (Cast<ASATORICharacter>(Actor) != nullptr)
			{
				ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
				bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

				FRotator RotationOfIA = CurrentActorInfo->AvatarActor->GetActorRotation();
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


				ASATORI_RaijinRayoExtensible* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayoExtensible>(ProjectileClass,
					IA_POS,
					RotationOfIA);
				if(Rayo)
				{
					Rayo->Damage = Damage;
					Rayo1 = Rayo;
				}

				break;

				/*	ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(

						ProjectileClass,
						IATransform,
						RotationOfIA);*/


				UE_LOG(LogTemp, Display, TEXT("Bala creadaa"));
			}


			//ASATORI_ArcherProjectile* NewProjectile = World->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass, Transform, SpawnParams);
		}

		TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_AreaExtensible::EndRayos, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, true);

		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	}
}

void USATORI_AreaExtensible::EndRayos(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	//UE_LOG(LogTemp, Warning, TEXT("Num Rayos %f"), Rayos.Num());
	if (Rayo1)
	{
		if(Rayo1->destruible)
		{
			Rayo1->Destroy();

			ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());

			if (Raijin)
			{
				if (Raijin->ArcoAltavoces)
				{
					Raijin->ArcoAltavoces->setAttacking(false);
					//Raijin->EndPartArco();
				}
			}

			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		else
		{
			if(!Particula_loop->IsActive())
			{
				if (Area_Particle)
				{
					FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();
					Particula_loop = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
						Area_Particle, IA_POS, FRotator::ZeroRotator, true);

					
				}
			}

				
		}
		
	}
	

}

void USATORI_AreaExtensible::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());

	if (Raijin)
	{
		if (Raijin->ArcoAltavoces)
		{
			Raijin->ArcoAltavoces->setAttacking(false);
			//Raijin->EndPartArco();
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void USATORI_AreaExtensible::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	/*ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
	if (Raijin)
	{
		Raijin->ArcoAltavoces->setAttacking(false);
	}*/
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


