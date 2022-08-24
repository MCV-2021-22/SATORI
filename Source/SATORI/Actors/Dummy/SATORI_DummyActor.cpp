// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Dummy/SATORI_DummyActor.h"

// Sets default values
ASATORI_DummyActor::ASATORI_DummyActor()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dummy"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASATORI_DummyActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASATORI_DummyActor::PlayAnimationsWithAttack()
{
	// TODO 
	// Animation Code Here
}