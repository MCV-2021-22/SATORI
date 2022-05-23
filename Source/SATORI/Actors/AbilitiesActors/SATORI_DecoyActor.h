//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
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

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float SphereRadiusOfAction = 600.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Decoy")
	FName TagGrantedWhenLured;

	UFUNCTION(BlueprintCallable, Category = "Missile")
		void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FName TargetActorWithTag = FName(TEXT("State.Targeted"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FName EnemyTag = FName(TEXT("Enemy"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decoy|Tags")
	FName PlayerTag = FName(TEXT("Player"));

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:

	TArray<AActor*> ArrayLured;

	FTimerHandle TimerHandleDestroy;

	void DestroyMyself();

};
