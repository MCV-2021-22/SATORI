// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectileSpawnerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ASATORI_RangeMovable::ASATORI_RangeMovable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	//CharacterMovement->bRunPhysicsWithNoController = true;

	//CharMoveComp

	//uint8 bRunPhysicsWithNoController : 1;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	//CharacterMovement->bRunPhysicsWithNoController = 3;

	bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/RangeMovable/BT_RangeMovable.BT_RangeMovable")));
	
}

void ASATORI_RangeMovable::Tick(float DeltaTime)
{


	Super::Tick(DeltaTime);
	float Value = 1.0f;


	if ((Controller != nullptr) && (Value != 0.0f) && moveBackwards)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, -Value);
	}
	else if ((Controller != nullptr) && (Value != 0.0f) && moveLeft) {
		// find out which way is forward
		TArray< AActor* > enemigos;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
		for (AActor* Actor : enemigos)
		{
			if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
			{

				ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);

				FVector fwd = Player->GetActorLocation() - GetActorLocation();

				FVector dest1 = GetActorLocation() + fwd;

				//FVector newForward2 = dest - Sphere2->GetActorLocation();
				FVector RotateValue2 = fwd.RotateAngleAxis(15.0f, FVector(0, 0, 1));
				//RotateValue2.Normalize();
				
				//Sphere2->setDirection(RotateValue2 * 20);

				FVector dest2 = dest1 - RotateValue2;

				//const FRotator Rotation = Controller->GetControlRotation();
				//const FRotator YawRotation(0, Rotation.Yaw + 15 , 0);




				// get forward vector
				//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

				FVector dest3 = dest2 - GetActorLocation();

				AddMovementInput(dest3, Value);
				
				break;

			}
		}

		
	}
	else if ((Controller != nullptr) && (Value != 0.0f) && moveRight)
	{
		// find out which way is forward
		TArray< AActor* > enemigos;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
		for (AActor* Actor : enemigos)
		{
			if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
			{

				ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);

				FVector fwd = Player->GetActorLocation() - GetActorLocation();

				FVector dest1 = GetActorLocation() + fwd;

				//FVector newForward2 = dest - Sphere2->GetActorLocation();
				FVector RotateValue2 = fwd.RotateAngleAxis(-15.0f, FVector(0, 0, 1));
				//RotateValue2.Normalize();

				//Sphere2->setDirection(RotateValue2 * 20);

				FVector dest2 = dest1 - RotateValue2;

				//const FRotator Rotation = Controller->GetControlRotation();
				//const FRotator YawRotation(0, Rotation.Yaw + 15 , 0);




				// get forward vector
				//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

				FVector dest3 = dest2 - GetActorLocation();

				AddMovementInput(dest3, Value);

				break;

			}
		}
	}

}

float ASATORI_RangeMovable::getDistAttack()
{
	return dist_attack;
}



float ASATORI_RangeMovable::getCloseDist()
{
	return close_dist;
}