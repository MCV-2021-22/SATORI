//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "SATORI_BlackHoleActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float SphereRadiusOnExplosion;

	UPROPERTY(BlueprintReadWrite, Category = "BlackHole")
	bool Active = false;

	UFUNCTION(BlueprintCallable, Category = "BlackHole")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "BlackHole")
	void OnOverlapSphereOnExplosion(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "BlackHole")
	void Explode();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  TargetActorWithTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  PlayerTag;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	AActor* Target;

	APlayerController* Controller;

	TArray<UPrimitiveComponent*> ArrayTrapped;

	bool Exploded = false;

	FTimerHandle TimerHandleDestroy;

	void DestroyMyself();

};
