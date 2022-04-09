// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_AbilityMask.h"

// Sets default values for this component's properties
USATORI_AbilityMask::USATORI_AbilityMask()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USATORI_AbilityMask::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USATORI_AbilityMask::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

