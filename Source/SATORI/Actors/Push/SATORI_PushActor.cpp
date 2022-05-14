
#include "Actors/Push/SATORI_PushActor.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "SATORICharacter.h"

ASATORI_PushActor::ASATORI_PushActor()
{
	//Default
	SphereRadius = 32.0f;
	Speed = 2000.0f;
	TimeToDestroy = 1.5f;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(SphereRadius);
	SphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	RootComponent = SphereComponent;
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PushActor::OnOverlapSphere);

	//Debug
	SphereComponent->bHiddenInGame = false;

}

void ASATORI_PushActor::BeginPlay() {

	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASATORI_PushActor::OnTimerExpiredDestroy, TimeToDestroy, false);
}

void ASATORI_PushActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector Start = GetActorLocation();
	SetActorLocation(Start + GetActorForwardVector() * Speed * DeltaTime);

	for (UPrimitiveComponent* RootComp : ArrayPushed) {
		RootComp->SetWorldLocation(RootComp->GetComponentLocation() + GetActorForwardVector());
	}

	FHitResult OutHit;
	FVector End = Start;
	End.Z -= 250;
	FCollisionQueryParams CollisionParams;
	UWorld* World = GetWorld();
	bool bHitAnything = World->LineTraceSingleByProfile(OutHit, Start, End, FName("BlockOnlyStatic"), CollisionParams);
	//bool bHitAnything = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);
	//DrawDebugLine(World, Start, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
	if (bHitAnything) {
	//UPrimitiveComponent* Debug = Cast<UPrimitiveComponent>(OutHit.GetComponent());
	//UE_LOG(LogTemp, Display, TEXT("Hitting: %s"), *Debug->GetName());
		if (OutHit.Distance > 50) {
			Start.Z -= 10;
			SetActorLocation(Start);
		}
		if (OutHit.Distance < 30) {
			Start.Z += 10;
			SetActorLocation(Start);
		}
	}

}

void ASATORI_PushActor::OnTimerExpiredDestroy()
{	
	Destroy();
}

void ASATORI_PushActor::OnOverlapSphere(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{

	//TO DO: add Tag canbpushed
	if (OtherActor->ActorHasTag(FName("Enemy"))) {
		ArrayPushed.AddUnique(Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()));
	}
	if(!OtherActor->ActorHasTag(FName("Player")) && !OtherActor->ActorHasTag(FName("Enemy"))){
		Destroy();
	}

}