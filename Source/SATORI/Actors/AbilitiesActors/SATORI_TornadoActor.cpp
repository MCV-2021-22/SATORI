//

#include "Actors/AbilitiesActors/SATORI_TornadoActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
//Debug
#include "DrawDebugHelpers.h"
#include "AI/Character/Spawner/SATORI_Spawner.h"

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
	ProjectileMovementComponent->bConstrainToPlane = true;
	ProjectileMovementComponent->ConstrainNormalToPlane(FVector(0, 0, 1));
	ProjectileMovementComponent->bShouldBounce = true;

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Tornado Particle Effect"));
	ParticleEffect->SetupAttachment(RootComponent);
	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_TornadoActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	//Possible collisions : 

	//Objects
	if (!Character)
	{
		return;
	}

	

	//Enemies
	if (Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(PushedTag) && ArrayActorsTrapped.Num() < MaxEnemies)
	{
		StopAction(Character);
		ArrayActorsTrapped.AddUnique(OtherActor);
		Character->AddGameplayTag(PushedTag);
		CalculateAngle(OtherActor);
	}
}

//Stops ability and  animation if active
void ASATORI_TornadoActor::StopAction(ASATORI_AICharacter* Character)
{
	//Edge case tornado affects even if blocking
	Character->RemoveGameplayTag(NoDamageTag);
	//Edge Cases
	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(SpecialTag);
	GameplayTagContainer.AddTag(StunnedTag);
	USATORI_BlueprintLibrary::RemoveGameplayEffect(Character, GameplayTagContainer);

	Character->RemoveGameplayTag(AbilityTag);
	UAnimMontage* AnimMontage = Character->GetCurrentMontage();
	if (IsValid(AnimMontage))
	{
		Character->StopAnimMontage(AnimMontage);
	}
}

void ASATORI_TornadoActor::CalculateAngle(AActor* Actor) 
{
	FVector A = GetActorForwardVector();
	A.Z = 0;
	A.Normalize();
	FVector B = Actor->GetActorLocation() - GetActorLocation();
	B.Z = 0;
	B.Normalize();
	float  Dot = FVector::DotProduct(A, B);
	float Angle = FMath::Acos(Dot);

	ArrayAngleAxis.Add(Angle);
}

void ASATORI_TornadoActor::DestroyMyself()
{
	for (AActor* Actor : ArrayActorsTrapped) {

		if (IsValid(Actor))
		{
			LaunchEnemy(Actor);
		}
	}

	Destroy();
}

void ASATORI_TornadoActor::LaunchEnemy(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	Character->RemoveGameplayTag(PushedTag);

	FVector LaunchDirection = Actor->GetActorLocation() - GetActorLocation();
	LaunchDirection.Z = ZLaunching;
	LaunchDirection.Normalize();

	Character->LaunchCharacter(LaunchDirection * LaunchForce, true, true);

	Character->AddGameplayTag(LaunchTag);
}

void ASATORI_TornadoActor::BeginPlay()
{
	Super::BeginPlay();

	if (!EnemyTag.IsValid() || !PushedTag.IsValid() || !LaunchTag.IsValid() || !AbilityTag.IsValid() || !SpecialTag.IsValid() || !StunnedTag.IsValid() || !NoDamageTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TornadoActor: Tag not valid ... "), *GetName());
		Destroy();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASATORI_TornadoActor::DestroyMyself, TimeToFinish, false);
}

void ASATORI_TornadoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StayGrounded(DeltaTime);

	float Num = 0;

	for (AActor* Actor : ArrayActorsTrapped)
	{
		if (IsValid(Actor))
		{
			ASATORI_Spawner* Spawner = Cast<ASATORI_Spawner>(Actor);
			DamageEnemy(Actor);
			if (!Spawner)
			{
				MoveTrappedEnemies(DeltaTime, Actor, Num);
				RotateEnemy(Actor);
			}
				
			Num++;
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
		if (OutHit.Distance < MinDistanceToGround) {
			ActorPosition.Z += HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
		if (OutHit.Distance > MaxDistanceToGround) {
			ActorPosition.Z -= HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
	}
}

//Damage Calculation
void ASATORI_TornadoActor::DamageEnemy(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
	Character->CheckDamage(Damage);
}

//Position of enemies calculations
void ASATORI_TornadoActor::MoveTrappedEnemies(float DeltaTime, AActor* Actor, int Num)
{
	FVector CenterPosition = GetActorLocation();

	ArrayAngleAxis[Num] += SpeedRotation * DeltaTime * FMath::Log2(Num + DifferenceRotationSpeed);
	if (ArrayAngleAxis[Num] >= 360) ArrayAngleAxis[Num] = 0;

	FVector RotateValue = Dimensions.RotateAngleAxis(ArrayAngleAxis[Num], AxisVector);

	CenterPosition.X += RotateValue.X;
	CenterPosition.Y += RotateValue.Y;
	CenterPosition.Z += RotateValue.Z;

	Actor->SetActorLocation(CenterPosition, false, 0, ETeleportType::TeleportPhysics);
}

//Rotation of enemiescalculations
void ASATORI_TornadoActor::RotateEnemy(AActor* Actor)
{
	FVector RotationDirection = GetActorLocation() - Actor->GetActorLocation();
	RotationDirection.Normalize();
	FRotator Rotator = RotationDirection.Rotation();
	Actor->SetActorRotation(Rotator);
}