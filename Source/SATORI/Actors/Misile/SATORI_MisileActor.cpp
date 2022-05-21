// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Misile/SATORI_MisileActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ASATORI_MisileActor::ASATORI_MisileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereRadius = 128.0f;
	Speed = 6000.0f;
	TimeToDestroy = 2.5f;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(SphereRadius);
	SphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	RootComponent = SphereComponent;
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_MisileActor::OnOverlapSphere);

	//Debug
	SphereComponent->bHiddenInGame = false;

}

void ASATORI_MisileActor::OnOverlapSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag(FName("Enemy"))) {
		Destroy();
	}
	if (!OtherActor->ActorHasTag(FName("Player")) && !OtherActor->ActorHasTag(FName("Enemy"))) {
		Destroy();
	}

}

void ASATORI_MisileActor::OnTimerExpiredDestroy()
{

	Destroy();

}

// Called when the game starts or when spawned
void ASATORI_MisileActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASATORI_MisileActor::OnTimerExpiredDestroy, TimeToDestroy, false);
	
}

// Called every frame
void ASATORI_MisileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector Pos = GetActorLocation();
	SetActorLocation(Pos + GetActorForwardVector() * Speed * DeltaTime);



}

