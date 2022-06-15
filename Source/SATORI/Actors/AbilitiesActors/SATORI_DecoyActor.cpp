//

#include "Actors/AbilitiesActors/SATORI_DecoyActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
//Debug
#include "DrawDebugHelpers.h"

ASATORI_DecoyActor::ASATORI_DecoyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	//If collides will lure enemies
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("LuringRangeSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(StaticMeshComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DecoyActor::OnOverlapCollisionSphere);

	ExplosionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionRangeSphere"));
	ExplosionSphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	ExplosionSphereComponent->SetupAttachment(StaticMeshComponent);
	ExplosionSphereComponent->SetGenerateOverlapEvents(true);
	ExplosionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DecoyActor::OnOverlapExplosionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 200.0f;
	ProjectileMovementComponent->MaxSpeed = 200.0f;
	ProjectileMovementComponent->ProjectileGravityScale = -0.3f;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

}

//Collision for luring
void ASATORI_DecoyActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// BlackHole possible collisions : 
	// Enemies

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);
	
	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		Character->AddGameplayTag(LuredTag);
		ArrayLured.AddUnique(OtherActor);
	}
}

void ASATORI_DecoyActor::OnOverlapExplosionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// BlackHole possible collisions : 
	// Enemies

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Character->sendDamage(DamageDone);
	}
}

void ASATORI_DecoyActor::DestroyMyself()
{
	Destroy();
}

void ASATORI_DecoyActor::Explode()
{
	for (AActor* Actor : ArrayLured)
	{
		ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
		if (Character->GetHealth() > 0.0f)
		{
			Character->RemoveGameplayTag(LuredTag);
		}
	}

	ExplosionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")), true);

	GetWorldTimerManager().SetTimer(TimerHandleDestroyWait, this, &ASATORI_DecoyActor::DestroyMyself, 0.2f, false);
}

void ASATORI_DecoyActor::BeginPlay()
{
	Super::BeginPlay();

	//Set max time before auto destruc
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_DecoyActor::Explode, TimeToDestroy, false);
}

void ASATORI_DecoyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		if (OutHit.Distance > MaxHeight) {
			ActorPosition.Z -= HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
			ProjectileMovementComponent->ProjectileGravityScale = GravityDescending;
			ChangeDirection(true);
		}
		if (OutHit.Distance < MinHeight) {
			ActorPosition.Z += HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
			ProjectileMovementComponent->ProjectileGravityScale = GravityAscending;
			bChangedDirection = false;
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
