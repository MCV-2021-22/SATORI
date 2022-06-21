// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/GA_ArquerosMisil.h"

#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

UGA_ArquerosMisil::UGA_ArquerosMisil()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ArquerosMisil::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	FVector IA_POS = ActorInfo->AvatarActor->GetActorLocation();    

	TArray< AActor* > enemigos;
	

	FName tag = "PossessedBy.Player";

	if (IsClone)
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character.Clone"), enemigos);
		
	}
	else
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
		
	}

	for (AActor* Actor : enemigos)
	{
		if(Cast<ASATORI_CharacterBase>(Actor) != nullptr)
		{
			ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

			FVector dest = Player->GetActorLocation();

			FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			FTransform IATransform = ActorInfo->AvatarActor->GetTransform();

			ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
				ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 100,
				RotationOfIA);

			if(Sphere)
			{
				FVector newForward = dest - Sphere->GetActorLocation();
				newForward.Normalize();
				Sphere->damage = this->damage;
				Sphere->setDirection(newForward * 20);
			}
			break;
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}



