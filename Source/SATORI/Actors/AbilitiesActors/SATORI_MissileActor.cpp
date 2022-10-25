//

#include "Actors/AbilitiesActors/SATORI_MissileActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "SATORICharacter.h"
#include "SATORIGameMode.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASATORI_MissileActor::ASATORI_MissileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	//If collides will explode
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_MissileActor::OnOverlapCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->bIsHomingProjectile = true;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

	// Particle
	Missile_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MissileParticle"));
	Missile_Particle->SetupAttachment(RootComponent);
}

//Collision for exploding
void ASATORI_MissileActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Missile possible collisions : 

	CollisionSphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	FVector SpawnLocation = CollisionSphereComponent->GetComponentLocation();

	// Walls
	if (!Character)
	{
		if (Missile_Hit_Particle)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Missile_Hit_Particle, SpawnLocation);
		}
		DestroyMyself();
		return;
	}

	// Enemies
	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		if (Missile_Hit_Particle)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Missile_Hit_Particle, SpawnLocation);
		}
		DamageEnemy(OtherActor);
	}

	DestroyMyself();
}

void ASATORI_MissileActor::DestroyMyself()
{
	Destroy();
}

void ASATORI_MissileActor::BeginPlay()
{
	Super::BeginPlay();

	FindTarget();

	//Set max time before auto destruc if not collides
	FTimerHandle TimerHandleDestroy;
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_MissileActor::DestroyMyself, TimeToFinish, false);
}

void ASATORI_MissileActor::FindTarget()
{
	ProjectileMovementComponent->HomingTargetComponent = nullptr;

	ASATORICharacter* Player = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float  MissileRange = Range;
	Target = nullptr;

	if (Player->GetTargetSystemComponent()->IsLocked())
	{
		Target = Player->GetTargetSystemComponent()->GetLockedOnTargetActor();
	}
	else
	{
		TArray < AActor* >& AllEnemies = GetWorld()->GetAuthGameMode<ASATORIGameMode>()->GetEnemyActorsRef();

		for (AActor* Actor : AllEnemies)
		{
			const float Distance = GetDistanceTo(Actor);
			if (Distance < MissileRange && Player->GetTargetSystemComponent()->IsInViewport(Actor))
			{
				MissileRange = Distance;
				Target = Actor;
			}
		}
	}

	if (Target)
	{
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	}
}

//Damage Calculation
void ASATORI_MissileActor::DamageEnemy(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
	USATORI_BlueprintLibrary::ApplyGameplayEffect(Actor, StunGameplayEffect);
	Character->CheckDamage(Damage);
}