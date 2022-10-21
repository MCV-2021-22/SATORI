//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameplayEffect.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_DecoyActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
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

	// Particles
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particle")
	UNiagaraComponent* Decoy_Particle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Decoy")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decoy")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float TimeToFinish;

	//Movement Calculations
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float TraceDistanceToGround = 800.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float MinDistanceToGround = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float MaxDistanceToGround = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float HeightChange = 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float GravityAscending = -0.3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float GravityDescending = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float SideDeviation = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool bDrawDebug = false;

	UFUNCTION(BlueprintCallable, Category = "Decoy")
		void OnOverlapCollisionSphere(
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

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	void DestroyMyself();
	void StayGrounded(float DeltaTime);
	void ChangeDirection(bool ChangeDirection);
	bool bChangedDirection = false;

};
