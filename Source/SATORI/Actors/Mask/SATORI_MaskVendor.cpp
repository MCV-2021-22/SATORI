// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Mask/SATORI_MaskVendor.h"
#include "SATORI/SATORICharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASATORI_MaskVendor::ASATORI_MaskVendor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASATORI_MaskVendor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASATORI_MaskVendor::Interact(AActor* ActorInteracting)
{
	UE_LOG(LogTemp, Display, TEXT("Interacting with Mask Vendor"));
}

void ASATORI_MaskVendor::StartCanInteract(AActor* ActorInteracting)
{

}

void ASATORI_MaskVendor::StopCanInteract(AActor* ActorInteracting)
{
	
}

void ASATORI_MaskVendor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character)
	{

	}
}