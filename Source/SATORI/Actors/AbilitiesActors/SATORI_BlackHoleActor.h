//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
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
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "BlackHole")
	float TimeToDestroy;

	UFUNCTION(BlueprintCallable, Category = "BlackHole")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  TagToAddWhenTrapped;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BlackHole|Tags")
	FGameplayTag  PlayerTag;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	TArray<ASATORI_CharacterBase*> ArrayTrapped;

	FTimerHandle TimerHandleDestroy;

	void DestroyMyself();

};
