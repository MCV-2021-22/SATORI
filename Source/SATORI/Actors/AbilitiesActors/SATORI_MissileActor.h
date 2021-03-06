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


UCLASS()
class SATORI_API ASATORI_MissileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_MissileActor();

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Missile")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Missile")
	//UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Missile")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float Range;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Missile|Tags")
	FGameplayTag  EnemyTag;

protected:
	
	virtual void BeginPlay() override;

private:

	AActor* Target;

	FTimerHandle TimerHandleDestroy;

	void DestroyMyself();

};
