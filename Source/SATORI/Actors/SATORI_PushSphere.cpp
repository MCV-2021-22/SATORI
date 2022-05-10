// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SATORI_PushSphere.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASATORI_PushSphere::ASATORI_PushSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//This shouldn't be here
	Speed = 300.0f;
	SphereRadius = 32.0f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(SphereRadius);
	SphereComponent->SetCollisionProfileName(FName(TEXT("Trigger")));
	RootComponent = SphereComponent;
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PushSphere::OnOverlapSphere);
	//Debug
	SphereComponent->bHiddenInGame = false;

}

void ASATORI_PushSphere::OnOverlapSphere(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	//TO DO: add Tag canbpushed
	if (OtherComp->ComponentHasTag(FName("Enemy"))) {
		HandleHit(OtherActor);
	}

}

void ASATORI_PushSphere::HandleHit_Implementation(AActor* OtherActor)
{
	//TO DO: Use Tags for this
	if (OtherActor->IsRootComponentMovable()) {

		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());

		RootComp->SetSimulatePhysics(true);
		RootComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		FVector Forward = GetActorForwardVector();
		FVector Impulse = Forward * 200;

		RootComp->AddImpulse(Impulse * RootComp->GetMass());

		//Debug
		FVector PosStart = GetActorLocation();
		FVector PosEnd = PosStart + Impulse;
		UWorld* World = GetWorld();
		DrawDebugLine(World, PosStart, PosEnd, FColor::Red, false, 5.0f);

	}

}

void ASATORI_PushSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);

}

//TO DO: Improve destroy of actor
void ASATORI_PushSphere::delayedDestroy(float Delay)
{

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, 
		this, 
		&ASATORI_PushSphere::DestroyMyself,
		Delay, 
		false);

}

void ASATORI_PushSphere::DestroyMyself() {

	this->Destroy();

}


