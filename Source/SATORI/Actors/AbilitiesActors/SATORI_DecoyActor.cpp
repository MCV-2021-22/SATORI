//

#include "Actors/AbilitiesActors/SATORI_DecoyActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "NiagaraComponent.h"
//Debug
#include "DrawDebugHelpers.h"

ASATORI_DecoyActor::ASATORI_DecoyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetVisibility(false);
	RootComponent = StaticMeshComponent;

	//If collides will lure enemies
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("LuringRangeSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DecoyActor::OnOverlapCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 200.0f;
	ProjectileMovementComponent->MaxSpeed = 200.0f;
	ProjectileMovementComponent->ProjectileGravityScale = -0.3f;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

	// Particles
	Decoy_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PushParticle"));
	Decoy_Particle->SetupAttachment(RootComponent);
}

//Collision for luring
void ASATORI_DecoyActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Decoy possible collisions : 

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);
	
	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(LuredTag))
	{
		Character->AddGameplayTag(LuredTag);
		ArrayLured.AddUnique(OtherActor);
	}
}


void ASATORI_DecoyActor::DestroyMyself()
{
	for (AActor* Actor : ArrayLured)
	{
		if (IsValid(Actor))
		{
			ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
			Character->RemoveGameplayTag(LuredTag);
		}
	}
	Destroy();
}

void ASATORI_DecoyActor::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Ability.Decoy");

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASATORI_DecoyActor::DestroyMyself, TimeToFinish, false);
}

void ASATORI_DecoyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StayGrounded(DeltaTime);
}

//Stay grounded calculation
void ASATORI_DecoyActor::StayGrounded(float DeltaTime)
{
	FVector ActorPosition = GetActorLocation();
	FVector End = ActorPosition;
	End.Z -= TraceDistanceToGround;
	bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, ActorPosition, End, FName("BlockOnlyStatic"), CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), ActorPosition, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
	}
	if (bHitAnything) {
		if (OutHit.Distance < MinDistanceToGround) {
			ActorPosition.Z += HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
			ProjectileMovementComponent->ProjectileGravityScale = GravityAscending;
			bChangedDirection = false;
		}
		if (OutHit.Distance > MaxDistanceToGround) {
			ActorPosition.Z -= HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
			ProjectileMovementComponent->ProjectileGravityScale = GravityDescending;
			ChangeDirection(true);
		}
	}
}

void ASATORI_DecoyActor::ChangeDirection(bool ChangeDirection)
{
	if (!bChangedDirection)
	{
		if(FMath::RandBool())
		{ 
			ProjectileMovementComponent->Velocity.Y = ProjectileMovementComponent->Velocity.Y + SideDeviation;
		}
		else
		{
			ProjectileMovementComponent->Velocity.Y = ProjectileMovementComponent->Velocity.Y - SideDeviation;
		}
	}
	bChangedDirection = ChangeDirection;
}
