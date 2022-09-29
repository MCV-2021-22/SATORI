// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/Raijin//SATORI_ArcoAltavoces.h"

// Sets default values
ASATORI_ArcoAltavoces::ASATORI_ArcoAltavoces()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASATORI_ArcoAltavoces::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASATORI_ArcoAltavoces::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASATORI_ArcoAltavoces::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ASATORI_ArcoAltavoces::getAttacking()
{
	return attacking;
}

void ASATORI_ArcoAltavoces::setAttacking(bool newState)
{
	attacking = newState;
}
