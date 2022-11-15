// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_AreasSala.h"

#include "SATORICharacter.h"
#include "Engine/DecalActor.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Kismet/GameplayStatics.h"

USATORI_AreasSala::USATORI_AreasSala()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_AreasSala::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AreasSala::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AreasSala::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AreasSala::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AreasSala::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AreasSala::EventReceived);
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
			if (!Raijin->Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Raijin.C2"))))
			{
				Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Raijin.C2"));
			}
			
		}
		
	}
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}




void USATORI_AreasSala::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();

		// Particles
		if (Area_Particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				Area_Particle, IA_POS, FRotator::ZeroRotator, true);
		}

		TArray< AActor* > enemigos;

		FName tag = "PossessedBy.Player";

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

		TArray<FVector> Array_Pos;
		//Array_Pos.SetNum(8);

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

				for (int i = 0; i < 8; i++)
				{
					bool spawned = false;

					while (!spawned)
					{
						spawned = true;

						if (i == 0)
						{
							ASATORI_RaijinRayo* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayo>(ProjectileClass,
								Player->GetActorLocation(),
								RotationOfIA);
							//UFMODBlueprintStatics::PlayEvent2D(GetWorld(), Attack3Effect, true);


							if(Rayo)
							{
								Rayo->Damage = Damage;
							}
						}
						else
						{
							int x = rand() % 2000 - 1000;
							int y = rand() % 2000 - 1000;

							FVector finalpos = Player->GetActorLocation() + FVector(x, y, 0);
							ASATORI_RaijinRayo* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayo>(ProjectileClass,
								finalpos,
								RotationOfIA);
							//UFMODBlueprintStatics::PlayEvent2D(GetWorld(), Attack3Effect, true);


							if (Rayo)
							{
								Rayo->Damage = Damage;
							}

							//miramos si colisiona en algun lugar, tanto con el borde del mapa como con los propios rayos
							if (!Rayo->CheckCollision())
							{
								for (int j = 0; j < Array_Pos.Num(); j++)
								{
									FVector vector_dist = Rayo->GetActorLocation() - Array_Pos[j];

									float dist = vector_dist.Dist(Rayo->GetActorLocation(), Array_Pos[j]);

									//float dist = sqrt(pow(vector_dist.X, 2) + pow(vector_dist.Y, 2) );

									if (dist < 300.0f)
									{
										spawned = false;
									}
								}
							}
							else
							{
								spawned = false;
							}

							//eliminamos
							if (!spawned)
							{
								Rayo->my_decal->Destroy();
								Rayo->Destroy();
							}
							else {
								Array_Pos.Add(Rayo->GetActorLocation());
							}
						}
					}
				}
				break;
			}
		}
		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	}
}

void USATORI_AreasSala::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
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


void USATORI_AreasSala::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
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