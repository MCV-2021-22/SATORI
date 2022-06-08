//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "SATORI_DashMeleeActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SATORI_API ASATORI_DashMeleeActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASATORI_DashMeleeActor();

	UPROPERTY(EditDefaultsOnly, Category = "DashMelee")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "DashMelee")
	USphereComponent* MeleeSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "DashMelee")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "DashMelee")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "DashMelee")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "DashMelee")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "DashMelee")
	float PushForce;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "DashMelee")
	float TimeToDestroy = 20.f;

	UFUNCTION(BlueprintCallable, Category = "DashMelee")
	void OnOverlapSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "DashMelee")
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "DashMelee")
		void OnOverlapSphereMelee(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DashMelee|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DashMelee|Tags")
	FGameplayTag  PlayerTag;

	virtual void Tick(float DeltaTime) override;


private:

	void DestroyMyself();

	float CurrentTime = 0.f;

	bool DamagePlayer = true;

	bool DestroyObject = false;

};
