// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BlackHole/SATORI_BlackHoleActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
ASATORI_BlackHoleActor::ASATORI_BlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereRadius = 128.0f;
	SphereRadiusOnExplosion = 1000.0f;
	Speed = 2000.0f;
	TimeToDestroy = 2.5f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(SphereRadius);
	SphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	RootComponent = SphereComponent;
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapSphere);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapSphereOnExplosion);

	//Debug
	SphereComponent->bHiddenInGame = false;

}

// Called when the game starts or when spawned
void ASATORI_BlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (IsValid(Controller)) {
		EnableInput(Controller);
	}

	Active = true;

	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASATORI_BlackHoleActor::OnTimerExpiredDestroy, TimeToDestroy, false);

}

// Called every frame
void ASATORI_BlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector Pos = GetActorLocation();
	if(!Exploded)
	SetActorLocation(Pos + GetActorForwardVector() * Speed * DeltaTime);

	if (Exploded) {
		for (UPrimitiveComponent* RootComp : ArrayTrapped) {
			FVector EnemyPos = RootComp->GetComponentLocation();
			RootComp->SetWorldLocation(EnemyPos + (Pos - EnemyPos));
		}
	}

}

void ASATORI_BlackHoleActor::OnOverlapSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag(FName("Enemy"))) {
		Explode();
	}
	if (!OtherActor->ActorHasTag(FName("Player")) && !OtherActor->ActorHasTag(FName("Enemy"))) {
		Explode();
	}

}

void ASATORI_BlackHoleActor::OnTimerExpiredDestroy()
{

	Destroy();

}

void ASATORI_BlackHoleActor::Explode() 
{


	GetWorldTimerManager().ClearTimer(UnusedHandle);

	Exploded = true;

	if (IsValid(Controller)) {
		DisableInput(Controller);
	}
	Active = false;

	SphereComponent->SetSphereRadius(SphereRadiusOnExplosion, true);

	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASATORI_BlackHoleActor::OnTimerExpiredDestroy, TimeToDestroy, false);

}

void ASATORI_BlackHoleActor::OnOverlapSphereOnExplosion(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Exploded) {
		if (OtherActor->ActorHasTag(FName("Enemy"))) {
			ArrayTrapped.AddUnique(Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()));
		}
	}
}