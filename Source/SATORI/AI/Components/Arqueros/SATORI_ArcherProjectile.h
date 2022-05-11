#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "SATORI_ArcherProjectile.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS(Blueprintable, Abstract)
class ASATORI_ArcherProjectile : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		int RemainingSeconds = 5;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

	ASATORI_ArcherProjectile();

	void BeginPlay() override;

	void Explode();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnExplosion();

	

protected:

	FTimerHandle ExplodeTimerHandle;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;
};



