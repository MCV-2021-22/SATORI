// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Arqueros/GA_ArqueroDefensivo.h"
#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "TimerManager.h"
#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "GameFramework/CharacterMovementComponent.h"


/*float value = -1.0f;
if ((Controller != nullptr) && (Value != 0.0f))
{
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}*/


UGA_ArqueroDefensivo::UGA_ArqueroDefensivo()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UGA_ArqueroDefensivo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());

	//Enemy = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	/*ASATORI_AICharacter* Enemy2 = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	ASATORI_AICharacter* Enemy3= Cast<ASATORI_AICharacter>(GetOwningActorFromActorInfo());


	if (Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo()) != nullptr)
	{
		Enemy = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
		//Enemy = Cast<ASATORI_AICharacter>(GetOwningActorFromActorInfo());

	}*/


	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	


	ASATORI_ArcherProjectile* Sphere = GetWorld()->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass,
		ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 100,
		RotationOfIA);

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

			FVector newForward = dest - Sphere->GetActorLocation();
			newForward.Normalize();

			Sphere->setDirection(newForward * 50);
			break;

		}

	}
	if(Defensive)
		Character->moveBackwards = true;

	TimerDelegate = FTimerDelegate::CreateUObject(this, &UGA_ArqueroDefensivo::OnTimerFinished, Handle, ActorInfo, ActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);

}

void UGA_ArqueroDefensivo::MoveBackwards(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());

	RemainingSeconds= RemainingSeconds-0.5;
	if (RemainingSeconds <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("SE ACABO EL MOVERSE!!"));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		RemainingSeconds = 5.5;

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);



	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%f secs to explosion"), RemainingSeconds);

		float value = -1.0f;
		if ((Character != nullptr) && (value != 0.0f))
		{
			UE_LOG(LogTemp, Display, TEXT("%f Es el range movable"), RemainingSeconds);
			// find out which way is forward
			const FRotator Rotation = Character->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			Character->AddMovementInput(Direction, -1.0f, false);

			//Character->GetCharacterMovement()->AddInputVector(Direction);
		}

		float Value = -1.0f;
		if ((Character->Controller != nullptr) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Character->Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			Character->AddMovementInput(Direction, Value);
		}

	}

	

}


void UGA_ArqueroDefensivo::OnTimerFinished(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	if(Defensive)
	{
		ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(GetAvatarActorFromActorInfo());
		Character->moveBackwards = false;
	}
	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);


}


void UGA_ArqueroDefensivo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}