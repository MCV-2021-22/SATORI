//

#include "Actors/AbilitiesActors/SATORI_BlackHoleActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
//Debug
#include "DrawDebugHelpers.h"

ASATORI_BlackHoleActor::ASATORI_BlackHoleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(StaticMeshComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapCollisionSphere);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->SetupAttachment(CollisionSphereComponent);
	RadialForceComponent->Radius = 512.0f;
	RadialForceComponent->Falloff = ERadialImpulseFalloff::RIF_Linear;
	RadialForceComponent->ForceStrength = -1000000.0f;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->ProjectileGravityScale = -0.01f;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_BlackHoleActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// BlackHole possible collisions : 
	// Enemies
	// Objects?

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character) 
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		Character->AddGameplayTag(TrappedTag);
		ArrayActorsTrapped.AddUnique(OtherActor);
		
		float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		//Character->sendDamage(DamageDone);
	}
}

void ASATORI_BlackHoleActor::DestroyMyself()
{
	for (AActor* Actor : ArrayActorsTrapped) {

		ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
		Character->RemoveGameplayTag(TrappedTag);

		Actor->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	Destroy();
}

void ASATORI_BlackHoleActor::StopGrowing()
{
	bGrowing = false;
	ProjectileMovementComponent->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(TimerHandleAttraction, this, &ASATORI_BlackHoleActor::StopAttraction, TimeToStopAttraction, false);
}

void ASATORI_BlackHoleActor::StopAttraction()
{
	bShouldAttract = false;
	RadialForceComponent->Radius = 64.0f;
	RadialForceComponent->ForceStrength = 500000.0f;

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_BlackHoleActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_BlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(ScaleGrowing);
	GetWorldTimerManager().SetTimer(TimerHandleGrowing, this, &ASATORI_BlackHoleActor::StopGrowing, TimeToStopGrowing, false);
}

void ASATORI_BlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGrowing)
	{
		ScaleGrowing = ScaleGrowing + (Increment * DeltaTime);
		SetActorScale3D(ScaleGrowing);

		//Stay grounded calculation
		FVector ActorPosition = GetActorLocation();
		FVector End = ActorPosition;
		End.Z -= TraceDistanceToFloor;
		bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, ActorPosition, End, FName("BlockOnlyStatic"), CollisionParams);
		if (bDrawDebug)
		{
			DrawDebugLine(GetWorld(), ActorPosition, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
		}
		if (bHitAnything) {
			if (OutHit.Distance < MinHeight) {
				ActorPosition.Z += HeightChange * DeltaTime;
				SetActorLocation(ActorPosition);
			}
		}
	}

	if (bShouldAttract)
	{
		for (AActor* Actor : ArrayActorsTrapped) {

			FVector CenterPosition = GetActorLocation();
			FVector VectorToCenter = Actor->GetActorLocation() - CenterPosition;

			FVector Rotation = VectorToCenter.RotateAngleAxis(DeltaTime * RotationSpeed, CenterPosition);
			Rotation.Normalize();

			Actor->AddActorWorldOffset(Rotation);

			VectorToCenter = Actor->GetActorLocation() - CenterPosition;
			Actor->SetActorLocation(Actor->GetActorLocation() - (VectorToCenter * DeltaTime));
			
			FVector Scale = Actor->GetActorScale3D();
			FVector NewScale = Scale - (Scale * DeltaTime * Decrement);

			Actor->SetActorScale3D(NewScale);
		}
	}
	else
	{
		ScaleGrowing = ScaleGrowing + (Increment * ExplosionSize * DeltaTime);
		SetActorScale3D(ScaleGrowing);
	}

}