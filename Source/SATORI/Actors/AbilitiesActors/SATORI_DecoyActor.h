//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SATORI_DecoyActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SATORI_API ASATORI_DecoyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_DecoyActor();

	UPROPERTY(EditDefaultsOnly, Category = "Decoy")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Decoy")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Decoy")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	FGameplayTag TagGrantedWhenLured;

	UFUNCTION(BlueprintCallable, Category = "Missile")
		void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FGameplayTag  TargetActorWithTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FGameplayTag  PlayerTag;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:

	AActor* Target;

	TArray<AActor*> ArrayLured;

	FTimerHandle TimerHandleDestroy;

	void DestroyMyself();

};
