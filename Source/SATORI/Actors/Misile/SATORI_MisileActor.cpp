// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Misile/SATORI_MisileActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASATORI_MisileActor::ASATORI_MisileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	float SphereRadius = 32.0f;
	float SeekingSphereRadius = 128.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(FName(UCollisionProfile::NoCollision_ProfileName));
	
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_MisileActor::OnOverlapCollisionSphere);

	SeekingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SeekingSphere"));
	SeekingSphereComponent->SetSphereRadius(SeekingSphereRadius);
	SeekingSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	SeekingSphereComponent->SetupAttachment(RootComponent);
	SeekingSphereComponent->SetGenerateOverlapEvents(true);
	SeekingSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_MisileActor::OnOverlapSeekingSphere);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
	SeekingSphereComponent->bHiddenInGame = false;
}

void ASATORI_MisileActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag(FName("Enemy"))) {
		Explode();
	}
	if (!OtherActor->ActorHasTag(FName("Player")) && !OtherActor->ActorHasTag(FName("Enemy"))) {
		Explode();
	}

}

void ASATORI_MisileActor::OnOverlapSeekingSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag(FName("Enemy")) && !Target) {
		Target = OtherActor;
	}

}

void ASATORI_MisileActor::Explode()
{

	Destroy();
}

// Called when the game starts or when spawned
void ASATORI_MisileActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_MisileActor::Explode, TimeToDestroy, false);
	
}

// Called every frame
void ASATORI_MisileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector Pos = GetActorLocation();
	if(Target)
	{
		FVector TargetPosition = Target->GetActorLocation();
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(Pos, TargetPosition);
		SetActorLocation(Pos + Direction * Speed * DeltaTime);
	}
	else
	{
		SetActorLocation(Pos + GetActorForwardVector() * Speed * DeltaTime);
	}
}

