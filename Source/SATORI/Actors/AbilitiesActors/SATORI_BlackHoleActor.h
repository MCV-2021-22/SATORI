//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_BlackHoleActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URadialForceComponent;
class UProjectileMovementComponent;

UCLASS()
class SATORI_API ASATORI_BlackHoleActor : public AActor
{
	GENERATED_BODY()

public:	

	ASATORI_BlackHoleActor();

	UPROPERTY(EditDefaultsOnly, Category = "BlackHole")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "BlackHole")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "BlackHole")
	URadialForceComponent* RadialForceComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackHole")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float TimeToStopGrowing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float TimeToStopAttraction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug|DistanceToGround")
	float TraceDistanceToFloor = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug|DistanceToGround")
	float HeightChange = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug|DistanceToGround")
	float MinHeight = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug|DistanceToGround")
	bool bDrawDebug = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float TimeToDestroy = 0.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	FVector ScaleGrowing = FVector(0.2f, 0.2f, 0.2f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float ExplosionSize = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float Increment = 1.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float Decrement = 0.75f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float RotationSpeed = 90.0f;

	UFUNCTION(BlueprintCallable, Category = "BlackHole")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  TrappedTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  EnemyTag;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	TArray<AActor*> ArrayActorsTrapped;

	FTimerHandle TimerHandleDestroy;
	FTimerHandle TimerHandleGrowing;
	FTimerHandle TimerHandleAttraction;

	void DestroyMyself();

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	bool bGrowing = true;
	void StopGrowing();

	bool bShouldAttract = true;
	void StopAttraction();
};
