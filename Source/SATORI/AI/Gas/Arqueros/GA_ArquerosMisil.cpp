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
	TArray< AActor* > enemigos2;

	FName tag = "PossessedBy.Player";

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORICharacter::StaticClass(), enemigos2);

	int array_dim = enemigos.Num();
	int array_dim2 = enemigos2.Num();

	UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), array_dim);

	UE_LOG(LogTemp, Display, TEXT("Number of actors2 with that tag: %d"), array_dim2);

	for (AActor* Actor : enemigos)
	{
		if(Cast<ASATORICharacter>(Actor) != nullptr)
		{
			ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
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
				Sphere->setDirection(newForward * 20);
			}
			break;
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}



