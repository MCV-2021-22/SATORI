// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_AreaExtensible.h"

#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_AreaExtensible::USATORI_AreaExtensible()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_AreaExtensible::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	FVector IA_POS = ActorInfo->AvatarActor->GetActorLocation();    

	TArray< AActor* > enemigos;

	FName tag = "PossessedBy.Player";

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORICharacter::StaticClass(), enemigos);

	for (AActor* Actor : enemigos)
	{
		//Actor->Tags.Add("PossessedBy.Player");
		if(Cast<ASATORICharacter>(Actor) != nullptr)
		{
			ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
		
			ASATORI_RaijinRayoExtensible* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayoExtensible>(ProjectileClass,
				IA_POS,
				RotationOfIA);

			break;
			
		/*	ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(

				ProjectileClass, 
				IATransform, 
				RotationOfIA);*/


			UE_LOG(LogTemp, Display, TEXT("Bala creadaa"));
		}
		

		//ASATORI_ArcherProjectile* NewProjectile = World->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass, Transform, SpawnParams);
	}


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}



