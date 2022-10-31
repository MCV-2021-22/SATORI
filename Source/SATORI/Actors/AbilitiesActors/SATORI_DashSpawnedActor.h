//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "AI/Character/Spawned/SATORI_Spawned.h"
#include "Components/BoxComponent.h"
#include "FMODBlueprintStatics.h"
#include "SATORI_DashSpawnedActor.generated.h"

class USphereComponent;
class UBoxComponent;
class UMaterialInterface;
class UDecalComponent;

UCLASS()
class SATORI_API ASATORI_DashSpawnedActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASATORI_DashSpawnedActor();

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	UDecalComponent* Decal = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	UBoxComponent* CollisionBoxComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Dash")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> HeavyHitGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* EventExplosion = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Dash")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Dash")
	AActor* OwnerSpawned;

	UFUNCTION(BlueprintCallable, Category = "Dash")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Dash")
		void OnOverlapCollisionBox(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dash|Tags")
	FGameplayTag  DashDamageTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dash|Tags")
	FGameplayTag  DashStopTag;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

private:

	ASATORI_Spawned* SpawnedCharacter;

	bool bDamagePlayer = false;
	bool bEndDash = false;

};
