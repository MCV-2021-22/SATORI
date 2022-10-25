//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "SATORI_MissileActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UGameplayEffect;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class SATORI_API ASATORI_MissileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_MissileActor();

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Missile")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Missile")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> StunGameplayEffect;

	// Particles
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particle")
	UNiagaraComponent* Missile_Particle = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particle")
	UNiagaraSystem* Missile_Hit_Particle = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float TimeToFinish;

	UFUNCTION(BlueprintCallable, Category = "Missile")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Missile|Tags")
	FGameplayTag  EnemyTag;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Missile")
	float Range;
	
	virtual void BeginPlay() override;

private:

	AActor* Target;

	void DestroyMyself();
	void FindTarget();
	void DamageEnemy(AActor* Actor);

};
