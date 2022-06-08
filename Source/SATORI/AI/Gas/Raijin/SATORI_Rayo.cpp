// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_Rayo.h"

#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_Rayo::USATORI_Rayo()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_Rayo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{


	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_Rayo::OnBucleRayos, Handle, ActorInfo, ActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, true);




	
}



void USATORI_Rayo::OnBucleRayos(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	iteracion++;

	TArray< AActor* > enemigos;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

	for (AActor* Actor : enemigos)
	{
		//Actor->Tags.Add("PossessedBy.Player");
		if (Cast<ASATORICharacter>(Actor) != nullptr)
		{
			ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

			FVector dest = Player->GetActorLocation();

			FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform IATransform = ActorInfo->AvatarActor->GetTransform();


			ASATORI_RaijinRayo* Rayo = GetWorld()->SpawnActor<ASATORI_RaijinRayo>(ProjectileClass,
				Player->GetActorLocation(),
				RotationOfIA);


			break;

		}

	}


	if (iteracion == max_iteracion)
	{
		
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		iteracion = 0;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	}


}