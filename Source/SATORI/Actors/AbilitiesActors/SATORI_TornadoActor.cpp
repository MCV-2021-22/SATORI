//

#include "Actors/AbilitiesActors/SATORI_TornadoActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
//Debug
#include "DrawDebugHelpers.h"

ASATORI_TornadoActor::ASATORI_TornadoActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetVisibility(false);
	RootComponent = StaticMeshComponent;

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_TornadoActor::OnOverlapCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_TornadoActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Tornado possible collisions : 
	// Enemies
	// Objects?

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(TrappedTag) && ArrayActorsTrapped.Num() < 2)
	{
		Character->AddGameplayTag(TrappedTag);
		ArrayActorsTrapped.AddUnique(OtherActor);

		if (ArrayActorsTrapped.Num() == 2)
		{
			SpeedRotation = SpeedRotation / 2;
		}

	}
}

void ASATORI_TornadoActor::DestroyMyself()
{
	for (AActor* Actor : ArrayActorsTrapped) {

		if (IsValid(Actor))
		{
			FinalActions(Actor);
		}
	}

	Destroy();
}

void ASATORI_TornadoActor::FinalActions(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	Character->RemoveGameplayTag(TrappedTag);

	Actor->SetActorLocation(GetActorLocation());

	FVector RandomDirection = FMath::VRand();
	RandomDirection.Z = 0.0f;
	Actor->AddActorLocalOffset(RandomDirection * RandomDirectionOffset);

	FVector LaunchDirection = Actor->GetActorLocation() - GetActorLocation();
	LaunchDirection.Normalize();
	Character->LaunchCharacter(LaunchDirection * LaunchForce, false, false);
}

void ASATORI_TornadoActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandleFinish, this, &ASATORI_TornadoActor::DestroyMyself, TimeToFinish, false);
}

void ASATORI_TornadoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StayGrounded(DeltaTime);

	for (AActor* Actor : ArrayActorsTrapped)
	{
		if (IsValid(Actor))
		{
			DamageTrappedEnemies(DeltaTime, Actor);
			MoveTrappedEnemies(DeltaTime, Actor);
		}
	}
}

//Stay grounded calculation
void ASATORI_TornadoActor::StayGrounded(float DeltaTime)
{
	FVector ActorPosition = GetActorLocation();
	FVector Ground = ActorPosition;
	Ground.Z -= TraceDistanceToGround;
	bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, ActorPosition, Ground, FName("BlockOnlyStatic"), CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), ActorPosition, Ground, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
	}
	if (bHitAnything) {
		if (OutHit.Distance < DistanceToGround) {
			ActorPosition.Z += HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
		if (OutHit.Distance > DistanceToGround + 25) {
			ActorPosition.Z -= HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
	}
}

//Damage Calculation
void ASATORI_TornadoActor::DamageTrappedEnemies(float DeltaTime, AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
	Character->sendDamage(DamageDone);
}

//Movement Calculations
void ASATORI_TornadoActor::MoveTrappedEnemies(float DeltaTime, AActor* Actor)
{
	FVector CenterPosition = GetActorLocation();
	AngleAxis += SpeedRotation * DeltaTime;
	if (AngleAxis >= 360) AngleAxis = 0;

	FVector RotateValue = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);

	CenterPosition.X += RotateValue.X;
	CenterPosition.Y += RotateValue.Y;
	CenterPosition.Z += RotateValue.Z;

	Actor->SetActorLocation(CenterPosition, false, 0, ETeleportType::None);
}