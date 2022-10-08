//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_TornadoActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UParticleSystemComponent;

UCLASS()
class SATORI_API ASATORI_TornadoActor : public AActor
{
	GENERATED_BODY()

public:	

	ASATORI_TornadoActor();

	UPROPERTY(EditDefaultsOnly, Category = "Tornado")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Tornado")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tornado")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Tornado")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Tornado")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Tornado")
	float TimeToFinish;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	int MaxEnemies = 2;

	//Rotation 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float SpeedRotation = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float AngleAxis = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	FVector Dimensions = FVector(200, 0, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	FVector AxisVector = FVector(0, 0, 1);

	//Debug rotation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1"), Category = "Tornado")
	float DifferenceRotationSpeed = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float LaunchForce = 800.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float ZLaunching = 80.0f;

	//Movement
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float Speed = 500.0f;

	//Stay Grounded
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float TraceDistanceToGround = 500.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float MinDistanceToGround = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float MaxDistanceToGround = 250.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float HeightChange = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	bool bDrawDebug = false;

	UFUNCTION(BlueprintCallable, Category = "Tornado")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  PushedTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  LaunchTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  AbilityTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  StunnedTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  SpecialTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  NoDamageTag;

	// Particles
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* ParticleEffect = nullptr;
protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	TArray<AActor*> ArrayActorsTrapped;
	UPROPERTY()
	TArray<float> ArrayAngleAxis;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	void StopAction(ASATORI_AICharacter* Character);
	void LaunchEnemy(AActor* Actor);
	void CalculateAngle(AActor* Actor);
	void MoveTrappedEnemies(float DeltaTime, AActor* Actor, int Num);
	void RotateEnemy(AActor* Actor);
	void StayGrounded(float DeltaTime);
	void DamageEnemy(AActor* Actor);
	void DestroyMyself();

};
