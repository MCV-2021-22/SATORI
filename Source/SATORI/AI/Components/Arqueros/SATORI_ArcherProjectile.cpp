#include "SATORI_ArcherProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"

ASATORI_ArcherProjectile::ASATORI_ArcherProjectile()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(16.0f);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	SphereComponent->SetSimulatePhysics(true);
	

	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	
}

void ASATORI_ArcherProjectile::BeginPlay()
{
	const bool bRecursive = true;
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimerHandle, this, &ASATORI_ArcherProjectile::Explode, 1.0f, bRecursive);
}

void ASATORI_ArcherProjectile::Explode()
{
	RemainingSeconds--;
	if (RemainingSeconds <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Explodes!!"));
		// Stop the recursive timer
		GetWorld()->GetTimerManager().ClearTimer(ExplodeTimerHandle);
		BP_OnExplosion();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%d secs to explosion"), RemainingSeconds);
	}
}

void ASATORI_ArcherProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);

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

	return (InstigatorTagInterface->HasMatchingGameplayTag(AITag) && OtherTagInterface->HasMatchingGameplayTag(PlayerTag) ) ||
		(InstigatorTagInterface->HasMatchingGameplayTag(PlayerTag) && OtherTagInterface->HasMatchingGameplayTag(AITag));

}

