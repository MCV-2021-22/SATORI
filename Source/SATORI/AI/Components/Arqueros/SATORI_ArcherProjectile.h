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
	ASATORI_ArcherProjectile();

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;


	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

	

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void setDirection(FVector newDirection);


protected:



	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

};



