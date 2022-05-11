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
	SphereRadius = 64.0f;
	Power = 200.0f;
	Grounded = false;

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

		RootComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());

		FVector Forward = GetActorForwardVector();
		FVector Impulse = Forward * Power;

		//Physics way
		//RootComp->SetSimulatePhysics(true);
		//RootComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//RootComp->AddImpulse(Impulse * RootComp->GetMass());

		//No physics way
		//RootComp->SetWorldLocation(GetActorLocation() + Impulse, true, 0, ETeleportType::None);

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

	FVector Start = GetActorLocation();

	//Movement
	SetActorLocation(Start + GetActorForwardVector() * Speed * DeltaTime);

	//Testing
	if (RootComp) {
		RootComp->SetWorldLocation(GetActorLocation(), true, 0, ETeleportType::None);
	}

	if(Grounded){
	//Stay always at X distance from ground
	FHitResult OutHit;
	FVector End = Start;
	End.Z -= 150;
	FCollisionQueryParams CollisionParams(FName("BlockAll"));
	UWorld* World = GetWorld();
	bool bHitAnything = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);
	DrawDebugLine(World, Start, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);


	if (OutHit.Distance > 50) {
		Start.Z -= 10;
		SetActorLocation(Start);
	}
	if (OutHit.Distance < 30) {
		Start.Z += 10;
		SetActorLocation(Start);
	}
	}
	//if (bHitAnything) {
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *OutHit.GetComponent()->GetName()));
	//}



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


