// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_Charm.h"

#include "SATORICharacter.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_Charm::USATORI_Charm()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_Charm::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{


	FVector IA_POS = CurrentActorInfo->AvatarActor->GetActorLocation();

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
		if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
		{
			ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

			FVector dest = Player->GetActorLocation();

			FRotator RotationOfIA = CurrentActorInfo->AvatarActor->GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform IATransform = CurrentActorInfo->AvatarActor->GetTransform();

			ASATORI_FujinCharm* Sphere = GetWorld()->SpawnActor<ASATORI_FujinCharm>(Charm,
				CurrentActorInfo->AvatarActor->GetActorLocation() + CurrentActorInfo->AvatarActor->GetActorForwardVector() * 100,
				RotationOfIA);

			if (Sphere)
			{
				FVector newForward = dest - Sphere->GetActorLocation();
				newForward.Normalize();
				Sphere->damage = this->damage;
				Sphere->setDirection(newForward * 20);

				ASATORI_Fujin* Fujin1 = Cast<ASATORI_Fujin>(ActorInfo->AvatarActor);
				if(Fujin1)
				{
					Sphere->Fujin = Fujin1;
				}
				
			}
			break;
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}




	




