// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/GA_ArqueroDefensivo.h"
#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "TimerManager.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "GameFramework/CharacterMovementComponent.h"




UGA_ArqueroDefensivo::UGA_ArqueroDefensivo()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UGA_ArqueroDefensivo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{


	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
	if (Defensive)
		Character->moveBackwards = true;

	TimerDelegate = FTimerDelegate::CreateUObject(this, &UGA_ArqueroDefensivo::OnTimerFinished, Handle, ActorInfo, ActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, true);
	

	
	

}

void UGA_ArqueroDefensivo::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	iteracion++;
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
		ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 100,
		RotationOfIA);

	TArray< AActor* > enemigos;
	if (IsClone)
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character.Clone"), enemigos);
		/*for (AActor* Actor : enemigos)
		{
			//Actor->Tags.Add("PossessedBy.Player");
			if (Cast<ASATORI_AICharacter>(Actor) != nullptr)
			{
				ASATORI_AICharacter* Player = Cast<ASATORI_AICharacter>(Actor);
				bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

				FVector dest = Player->GetActorLocation();
				if (Sphere)
				{
					FVector newForward = dest - Sphere->GetActorLocation();
					newForward.Normalize();

					Sphere->setDirection(newForward * 20);
				}


				break;

			}

		}*/
	}
	else
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
		/*for (AActor* Actor : enemigos)
		{
			//Actor->Tags.Add("PossessedBy.Player");
			if (Cast<ASATORICharacter>(Actor) != nullptr)
			{
				ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
				bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

				FVector dest = Player->GetActorLocation();
				if (Sphere)
				{
					FVector newForward = dest - Sphere->GetActorLocation();
					newForward.Normalize();

					Sphere->setDirection(newForward * 20);
				}


				break;

			}

		}*/
	}

	for (AActor* Actor : enemigos)
	{
		//Actor->Tags.Add("PossessedBy.Player");
		if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
		{
			ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			FVector dest = Player->GetActorLocation();
			if(Sphere)
			{
				FVector newForward = dest - Sphere->GetActorLocation();
				newForward.Normalize();

				Sphere->setDirection(newForward * 20);
			}

			
			break;

		}

	}
	
	if(iteracion == 3)
	{
		if (Defensive)
		{
			//ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
			Character->moveBackwards = false;


			Character->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Burst"));
		}

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		iteracion = 0;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		
	}


}

void UGA_ArqueroDefensivo::OnEndAb(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_ArqueroDefensivo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}