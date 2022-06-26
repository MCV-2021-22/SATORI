//

#include "Actors/AbilitiesActors/SATORI_MissileActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "SATORICharacter.h"
#include "SATORIGameMode.h"
//#include "GameState/SATORI_GameState.h"

ASATORI_MissileActor::ASATORI_MissileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	StaticMeshComponent->SetupAttachment(RootComponent);
	
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
}

//Collision for exploding
void ASATORI_MissileActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Missile possible collisions : 
	// Enemies
	// Walls

	CollisionSphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	// Walls
	if (!Character)
	{
		DestroyMyself();
		return;
	}

	// Enemies
	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Character->sendDamage(DamageDone);
	}

	DestroyMyself();
}

void ASATORI_MissileActor::DestroyMyself()
{
	ProjectileMovementComponent->Velocity = (FVector::ZeroVector);
	Destroy();
}

void ASATORI_MissileActor::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->HomingTargetComponent = nullptr;

	ASATORICharacter* Player = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player->GetTargetSystemComponent()->IsLocked())
	{
		Target = Player->GetTargetSystemComponent()->GetLockedOnTargetActor();
	}
	else
	{
		TArray < AActor* >& AllEnemies = GetWorld()->GetAuthGameMode<ASATORIGameMode>()->GetEnemyActorsRef();
		//TArray < AActor* >& AllEnemies = GetWorld()->GetGameState<ASATORI_GameState>()->GetEnemyActorsRef();

		for (AActor* Actor : AllEnemies)
		{
			const float Distance = GetDistanceTo(Actor);
			if (Distance < Range && Player->GetTargetSystemComponent()->IsInViewport(Actor))
			{
				Range = Distance;
				Target = Actor;
			}
		}
	}

	if (Target)
	{
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	}

	//Set max time before auto destruc if not collides
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_MissileActor::DestroyMyself, TimeToDestroy, false);
}

