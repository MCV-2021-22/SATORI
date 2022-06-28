// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_InteractComponent.h"
#include "Interfaces/SATORI_InteractInterface.h"
#include "DrawDebugHelpers.h"	

// Sets default values for this component's properties
USATORI_InteractComponent::USATORI_InteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USATORI_InteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USATORI_InteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasAuthority())
	{
		AActor* Owner = GetOwner();

		FVector PosStart = Owner->GetActorLocation();
		FVector ActorForward = Owner->GetActorRotation().Vector();
		FVector PosEnd = PosStart + ActorForward * InteractionDistance;

		UWorld* World = GetWorld();
		FHitResult HitResult;

		// Test a ray with objects
		FCollisionQueryParams TraceParams(FName("CompInteract"), FCollisionQueryParams::GetUnknownStatId(), true, Owner);
		bool bHitAnything = World->LineTraceSingleByChannel(HitResult, PosStart, PosEnd, CollisionChannel, TraceParams);
		::DrawDebugLine(World, PosStart, PosEnd, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);

		// 
		TWeakObjectPtr<AActor> NewActor = bHitAnything ? HitResult.Actor : nullptr;

		// Did we change anything?
		if (NewActor == CurrentActor)
		{
			return;
		}

		// Check if the Actor implements the InteractInterface. No problem if NewActor is null
		ISATORI_InteractInterface* NewInteract = Cast<ISATORI_InteractInterface>(NewActor);

		// Notify the old actor, we no longer want to interact with him
		if (CurrentActor.IsValid())
		{
			ISATORI_InteractInterface* CurrentInteract = Cast<ISATORI_InteractInterface>(CurrentActor);
			CurrentInteract->StopCanInteract(Owner);
			CurrentActor = nullptr;
		}

		// If we have something to interact with, notify it to the actor
		if (NewInteract)
		{
			CurrentActor = NewActor;
			NewInteract->StartCanInteract(Owner);
		}

	}
}

void USATORI_InteractComponent::TryToInteract_Implementation()
{

}

