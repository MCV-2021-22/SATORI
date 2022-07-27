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

UCLASS()
class SATORI_API ASATORI_TornadoActor : public AActor
{
	GENERATED_BODY()

public:	

	ASATORI_TornadoActor();

	UPROPERTY(EditDefaultsOnly, Category = "Tornado")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Tornado")
	USphereComponent* CollisionSphereWallsComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Tornado")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tornado")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Tornado")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Tornado")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Tornado")
	float TimeToFinish;

	//Rotation 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float SpeedRotation = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float AngleAxis = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	FVector Dimensions = FVector(300, 0, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	FVector AxisVector = FVector(0, 0, 1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float RandomDirectionOffset = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado")
	float LaunchForce = 200.0f;

	//Movement
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float Speed = 500.0f;

	//Stay Grounded
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float TraceDistanceToGround = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float DistanceToGround = 100.0f;

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
	FGameplayTag  TrappedTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tornado|Tags")
	FGameplayTag  EnemyTag;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	void DestroyMyself();

	FTimerHandle TimerHandleFinish;

	UPROPERTY()
	TArray<AActor*> ArrayActorsTrapped;

	void StayGrounded(float DeltaTime);
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	void  DamageTrappedEnemies(float DeltaTime, AActor* Actor);

	void MoveTrappedEnemies(float DeltaTime, AActor* Actor);

	void FinalActions(AActor* Actor);

};
