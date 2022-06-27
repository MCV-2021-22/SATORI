
#include "Actors/AbilitiesActors/SATORI_PullActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "SATORICharacter.h"
#include "SATORIGameMode.h"
//#include "GameState/SATORI_GameState.h"

// Sets default values
ASATORI_PullActor::ASATORI_PullActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	StaticMeshComponent->SetupAttachment(RootComponent);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	//If collides will grab
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PullActor::OnOverlapCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->bIsHomingProjectile = true;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

}

//Collision for Grabing
void ASATORI_PullActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Pull possible collisions : 
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

		if (IsValid(Character))
		{
			Character->AddGameplayTag(PullingTag);
			Pulling = Character;
			ProjectileMovementComponent->HomingTargetComponent = Player->GetRootComponent();
		}
		else 
		{
			DestroyMyself();
		}
		GetWorldTimerManager().ClearTimer(TimerHandleDestroy);
	}
}

void ASATORI_PullActor::DestroyMyself()
{

	ProjectileMovementComponent->HomingTargetComponent = nullptr;
	ProjectileMovementComponent->StopMovementImmediately();

	if (Pulling && IsValid(Pulling))
	{
		Pulling->RemoveGameplayTag(PullingTag);
	}
	
	NiagaraComponent->DestroyComponent();

	Destroy();
}

// Called when the game starts or when spawned
void ASATORI_PullActor::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovementComponent->HomingTargetComponent = nullptr;
	
	Player = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraSystem,
		Player->GetHandComponent(),
		FName (TEXT("NAME_NONE")),
		Player->GetHandComponent()->GetComponentLocation(),
		Player->GetHandComponent()->GetComponentRotation(),
		EAttachLocation::KeepWorldPosition,
		false,
		true,
		ENCPoolMethod::None,
		true
	);

	float  PullRange = Range;
	Target = nullptr;

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
			if (Distance < PullRange && Player->GetTargetSystemComponent()->IsInViewport(Actor))
			{
				PullRange = Distance;
				Target = Actor;
			}
		}
	}

	if (Target)
	{
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	}

	//Set max time before auto destruc if not collides
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_PullActor::DestroyMyself, TimeToDestroy, false);
}

// Called every frame
void ASATORI_PullActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();

	NiagaraComponent->SetVectorParameter("User.Beam End", ActorPosition);

	//If has grabbed an enemy
	if (Pulling)
	{
		Pulling->GetRootComponent()->SetWorldLocation(ActorPosition);

		//If has reached the player
		if (FVector::Dist(Player->GetActorLocation(), ActorPosition) < FinalDistanceToPlayer) {
			DestroyMyself();
		}
	}
}
