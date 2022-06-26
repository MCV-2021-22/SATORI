// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/GA_ArqueroAbanico.h"
#include "SATORICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "TimerManager.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector.h"

UGA_ArqueroAbanico::UGA_ArqueroAbanico()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UGA_ArqueroAbanico::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray< AActor* > enemigos;

	if(IsClone)
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character.Clone"), enemigos);
		
	}
	else
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
		
	}

	for (AActor* Actor : enemigos)
	{
		//Actor->Tags.Add("PossessedBy.Player");
		if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
		{
			ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

			FVector dest = Player->GetActorLocation();

			//primer misil

			ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
				ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 200,
				RotationOfIA, SpawnParams);

			if(Sphere)
			{
				FVector newForward = dest - Sphere->GetActorLocation();

				newForward.Normalize();
				Sphere->damage = this->damage;
				Sphere->setDirection(newForward * 20);
			}
			

			// segundo misil

			ASATORI_ArcherProjectile* Sphere1 = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
				ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 150,
				RotationOfIA, SpawnParams);

			if (Sphere1)
			{
				FVector newForward1 = dest - Sphere1->GetActorLocation();
				FVector RotateValue1 = newForward1.RotateAngleAxis(15.0f, FVector(0, 0, 1));

				RotateValue1.Normalize();
				Sphere1->damage = this->damage;
				//Sphere1->setDirection(newForward1 * 50);
				Sphere1->setDirection(RotateValue1 * 20);
			}
		

			// tercer misil
			ASATORI_ArcherProjectile* Sphere2 = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
				ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 100,
				RotationOfIA, SpawnParams);
			if (Sphere2)
			{
				FVector newForward2 = dest - Sphere2->GetActorLocation();
				FVector RotateValue2 = newForward2.RotateAngleAxis(-15.0f, FVector(0, 0, 1));
				RotateValue2.Normalize();
				Sphere2->damage = this->damage;
				Sphere2->setDirection(RotateValue2 * 20);

			}
			break;
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}