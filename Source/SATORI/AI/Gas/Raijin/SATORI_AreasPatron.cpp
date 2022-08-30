// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_AreasPatron.h"

#include "SATORICharacter.h"
#include "AI/Components/Raijin/SATORI_RaijinRayoMovilSpawns.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_AreasPatron::USATORI_AreasPatron()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_AreasPatron::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	FVector IA_POS = ActorInfo->AvatarActor->GetActorLocation();    

	TArray< AActor* > Spawns;
	

	FName tag = "PossessedBy.Player";

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Raijin.SpawnMovil"), Spawns);


	int array_dim = Spawns.Num();



	for(int i =0;i<3;i++)
	{
		if(Spawns.Num()>0)
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

				FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FTransform IATransform = ActorInfo->AvatarActor->GetTransform();


				ASATORI_RaijinRayoMovil* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayoMovil>(ProjectileClass,
					Player->CapsuleComponentInicio->GetRelativeLocation(),
					RotationOfIA);

				if (Rayo)
				{
					FVector newForward = Player->CapsuleComponentFinal->GetRelativeLocation() - Player->CapsuleComponentInicio->GetRelativeLocation();
					newForward.Normalize();
					Rayo->setDirection(newForward);
					Rayo->CapsuleComponentFinal->SetRelativeLocation(Player->CapsuleComponentFinal->GetRelativeLocation());
					Spawns.Remove(Actor);
					//Spawns.RemoveAt(num);
				}

			}
		}
		
			

			//ASATORI_ArcherProjectile* NewProjectile = World->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass, Transform, SpawnParams);
		//}
	}

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}



