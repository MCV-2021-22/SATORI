//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORICharacter.h"
#include "SATORI_PullActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UGameplayEffect;
class UProjectileMovementComponent;

UCLASS()
class SATORI_API ASATORI_PullActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_PullActor();

	UPROPERTY(EditDefaultsOnly, Category = "Pull")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pull")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pull")
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Pull")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Pull")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Pull")
	float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Pull")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float FinalDistanceToPlayer = 150.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Pull")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pull|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pull|Tags")
	FGameplayTag  PullingTag;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	AActor* Target;
	ASATORICharacter* Player;

	ASATORI_AICharacter* Pulling = nullptr;

	FTimerHandle TimerHandleDestroy;

	void DestroyMyself();

};
