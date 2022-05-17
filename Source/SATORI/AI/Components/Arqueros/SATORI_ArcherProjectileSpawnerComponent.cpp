#include "SATORI_ArcherProjectileSpawnerComponent.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectile.h"

void USATORI_ArcherProjectileSpawnerComponent::Fire(const FTransform& Transform)
{
	UWorld* World = GetWorld();

	// Do some checks
	if (!World || !ProjectileClass)
		return;

	// Spawn a projectile of the class at the given position
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetOwner<APawn>();
	ASATORI_ArcherProjectile* NewProjectile = World->SpawnActor<ASATORI_ArcherProjectile>(ProjectileClass, Transform, SpawnParams);
}