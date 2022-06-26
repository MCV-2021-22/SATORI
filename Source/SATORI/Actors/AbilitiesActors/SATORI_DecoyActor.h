//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_DecoyActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UGameplayEffect;
class UProjectileMovementComponent;

UCLASS()
class SATORI_API ASATORI_DecoyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_DecoyActor();

	UPROPERTY(EditDefaultsOnly, Category = "Decoy")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Decoy")
	USphereComponent* ExplosionSphereComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Decoy")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decoy")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float TraceDistanceToFloor = 800.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float HeightChange = 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float MaxHeight = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float MinHeight = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float GravityAscending = -0.3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float GravityDescending = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float SideDeviation = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool bDrawDebug = false;

	UFUNCTION(BlueprintCallable, Category = "Missile")
		void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


	UFUNCTION(BlueprintCallable, Category = "Missile")
		void OnOverlapExplosionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FGameplayTag LuredTag;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:

	TArray<AActor*> ArrayLured;

	FTimerHandle TimerHandleDestroy;
	FTimerHandle TimerHandleDestroyWait;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	void Explode();
	void DestroyMyself();
	void ChangeDirection(bool ChangeDirection);
	bool bChangedDirection = false;

};
