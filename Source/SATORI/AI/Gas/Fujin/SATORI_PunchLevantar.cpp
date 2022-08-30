// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_PunchLevantar.h"

#include "SATORICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_PunchLevantar::USATORI_PunchLevantar()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_PunchLevantar::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{


	int a = 2;

	FVector IA_POS = ActorInfo->AvatarActor->GetActorLocation();

	TArray< AActor* > enemigos;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

	for (AActor* Actor : enemigos)
	{

		//Actor->Tags.Add("PossessedBy.Player");
		if (Cast<ASATORICharacter>(Actor) != nullptr)
		{
			ASATORICharacter* Player1 = Cast<ASATORICharacter>(Actor);
			bool tiene = Player1->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			FVector dest = Player1->GetActorLocation();

			FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform IATransform = ActorInfo->AvatarActor->GetTransform();


			ASATORI_FujinLevantarCollision* Empuje = GetWorld()->SpawnActor<ASATORI_FujinLevantarCollision>(ProjectileClass,
				dest,
				RotationOfIA);

			Empuje->PushPlayer(Player1);

			Player = Player1;
			
			break;

		}

	}
	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_PunchLevantar::checkCollisionPlayer, CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, true);

	

	
}

void USATORI_PunchLevantar::checkCollisionPlayer(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	if (Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.EndLevantado")))
	{
		Player->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.EndLevantado"));
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		Player->EnableInput(PlayerController);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	
}