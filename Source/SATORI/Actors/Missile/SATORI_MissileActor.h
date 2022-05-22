//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "SATORI_MissileActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SATORI_API ASATORI_MissileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_MissileActor();

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	USphereComponent* SeekingSphereComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Missile")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float TimeToDestroy;

	UFUNCTION(BlueprintCallable, Category = "Missile")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Missile")
	void OnOverlapSeekingSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile|Tags")
	FName TargetActorWithTag = FName(TEXT("State.Targeted"));

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Missile|Tags")
	FName EnemyTag = FName(TEXT("Enemy"));

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Missile|Tags")
	FName PlayerTag = FName(TEXT("Player"));


protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:

	AActor* Target;

	FTimerHandle TimerHandleDestroy;

	void Explode();

};
