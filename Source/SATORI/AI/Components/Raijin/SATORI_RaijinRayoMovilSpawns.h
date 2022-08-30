#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "Components/DecalComponent.h"

#include "SATORI_RaijinRayoMovilSpawns.generated.h"

class ATargetPoint;
class ASATORICharacter;
class ADecalActor;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UCapsuleComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, Abstract)
class ASATORI_RaijinRayoMovilSpawns : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_RaijinRayoMovilSpawns();

	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* CapsuleComponentInicio = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* CapsuleComponentFinal = nullptr;

	UPROPERTY(EditDefaultsOnly)
		ATargetPoint* pfinal = nullptr;
	


	UFUNCTION()
		void OnComponentBeginOverlapFinal(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


	virtual void BeginPlay() override;

	
};



