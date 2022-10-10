#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "Components/DecalComponent.h"

#include "SATORI_FujinSpawns.generated.h"

class ATargetPoint;
class ASATORICharacter;
class ADecalActor;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UCapsuleComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, Abstract)
class ASATORI_FujinSpawns : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_FujinSpawns();

	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* CapsuleComponentInicio = nullptr;





	virtual void BeginPlay() override;

	
};



