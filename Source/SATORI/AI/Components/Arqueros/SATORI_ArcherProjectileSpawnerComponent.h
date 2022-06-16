#pragma once

#include "CoreMinimal.h"
#include "SATORI_ArcherProjectileSpawnerComponent.generated.h"

class ASATORI_ArcherProjectile;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class USATORI_ArcherProjectileSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere)
		TSubclassOf<ASATORI_ArcherProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
		void Fire(const FTransform& Transform);
};
