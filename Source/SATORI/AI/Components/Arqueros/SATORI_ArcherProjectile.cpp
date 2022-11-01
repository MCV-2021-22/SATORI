#include "SATORI_ArcherProjectile.h"

#include "SATORICharacter.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASATORI_ArcherProjectile::ASATORI_ArcherProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(radius);
	//SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	
	SphereComponent->OnComponentHit.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentHit);
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentBeginOverlap);
	//SphereComponent->SetSimulatePhysics(true);

	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	Moving_Projectile = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Moving Projectile"));
	Moving_Projectile->SetupAttachment(RootComponent);

	TArray< AActor* > enemigos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
}

void ASATORI_ArcherProjectile::setDirection(FVector newDirection)
{
	this->direction = newDirection;
}


void ASATORI_ArcherProjectile::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}


void ASATORI_ArcherProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameInstanceRef->TimeStop)
	{
		FVector position = GetActorLocation();
		FVector new_pos = position + direction * velocity * 0.1;

		SetActorLocation(new_pos);
		RootComponent->SetWorldLocation(new_pos);

		inmunity -= DeltaTime * 0.1;
		LifeTime -= DeltaTime * 0.1;
	} 
	else if(GameInstanceRef->TimeSlow)
	{
			FVector position = GetActorLocation();
			FVector new_pos = position + direction * velocity * 0.25;

			SetActorLocation(new_pos);
			RootComponent->SetWorldLocation(new_pos);

			inmunity -= DeltaTime * 0.25;
			LifeTime -= DeltaTime * 0.25;
	}
	else
	{
			FVector position = GetActorLocation();
			FVector new_pos = position + direction * velocity;

			SetActorLocation(new_pos);
			RootComponent->SetWorldLocation(new_pos);

			inmunity -= DeltaTime;
			LifeTime -= DeltaTime;
	}

	if (LifeTime <= 0)
	{
		Destroy();
	}
	
}


void ASATORI_ArcherProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);

	Destroy();
	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/
}



bool ASATORI_ArcherProjectile::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
{
	if (!InstigatorTagInterface || !OtherTagInterface) {
		return false;
	}

	// #TODO Use team agent interface
	FGameplayTag AITag = FGameplayTag::RequestGameplayTag("PossessedBy.AI");
	FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("PossessedBy.Player");

	return (InstigatorTagInterface->HasMatchingGameplayTag(AITag) && OtherTagInterface->HasMatchingGameplayTag(PlayerTag)) ||
		(InstigatorTagInterface->HasMatchingGameplayTag(PlayerTag) && OtherTagInterface->HasMatchingGameplayTag(AITag));

}

void ASATORI_ArcherProjectile::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASATORI_ArcherProjectile* Choque = Cast<ASATORI_ArcherProjectile>(OtherActor);

	if(Choque && inmunity <= 0)
	{
		ASATORICharacter* Player = Cast<ASATORICharacter>(OtherActor);
		Destroy();
	}
	else if (!Choque )
	{
		ASATORICharacter* Player = Cast<ASATORICharacter>(OtherActor);
		if(Player)
		{
			float dmg_done= USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, damage, OtherActor, DamageGameplayEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(Player, HitGameplayEffect);
			UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ProjectileHit, this->GetActorTransform(), true);
		}
		// Particles
		if (Impact_Particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact_Particle, GetActorLocation(), FRotator::ZeroRotator, true);
		}
		Destroy();
	}
	
}

void ASATORI_ArcherProjectile::DestroySelfByParry()
{
	// TODO Adding Particle system here, David will do this
	Destroy();
}