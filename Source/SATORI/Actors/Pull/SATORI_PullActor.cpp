
#include "Actors/Pull/SATORI_PullActor.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "SATORICharacter.h"

// Sets default values
ASATORI_PullActor::ASATORI_PullActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default
	SphereRadius = 128.0f;
	SpeedForward = 4000.0f;
	SpeedPulling = 2000.0f;
	TimeToDestroy = 2.5f;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(SphereRadius);
	SphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	RootComponent = SphereComponent;
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PullActor::OnOverlapSphere);

	//Debug
	SphereComponent->bHiddenInGame = false;

}

// Called when the game starts or when spawned
void ASATORI_PullActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASATORI_PullActor::OnTimerExpiredDestroy, TimeToDestroy, false);

}

// Called every frame
void ASATORI_PullActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector Pos = GetActorLocation();

	if (Pulling) {

		FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		FVector dir = UKismetMathLibrary::GetDirectionUnitVector(Pos, PlayerPosition);

		SetActorLocation(Pos + dir * SpeedPulling * DeltaTime);

		Pos = GetActorLocation();
		Pulling->SetWorldLocation(Pos);


		if (FVector::Dist(PlayerPosition, Pos) < 100) {
			Destroy();
		}
	}
	else {
		SetActorLocation(Pos + GetActorForwardVector() * SpeedForward * DeltaTime);
	}
}


void ASATORI_PullActor::OnTimerExpiredDestroy()
{
	Destroy();
}

void ASATORI_PullActor::OnOverlapSphere(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	//TO DO: add Tag canbpulled
	if (OtherActor->ActorHasTag(FName("Enemy"))) {
		Pulling = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	}
	if (!OtherActor->ActorHasTag(FName("Player")) && !OtherActor->ActorHasTag(FName("Enemy"))) {
		Destroy();
	}

}