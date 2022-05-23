// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectileSpawnerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	float Value = -1.0f;


	if ((Controller != nullptr) && (Value != 0.0f) && moveBackwards)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

}
