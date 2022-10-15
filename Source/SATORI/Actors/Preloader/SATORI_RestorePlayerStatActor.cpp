// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Preloader/SATORI_RestorePlayerStatActor.h"
#include "Components/CapsuleComponent.h"
#include "SATORICharacter.h"

// Sets default values
ASATORI_RestorePlayerStatActor::ASATORI_RestorePlayerStatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	Trigger->InitCapsuleSize(55.f, 96.0f);;
	Trigger->SetCollisionProfileName(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_RestorePlayerStatActor::OnOverlapBegin);

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	ActorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASATORI_RestorePlayerStatActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASATORI_RestorePlayerStatActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->RestartStats();
		}
		this->Destroy();
	}
}


