//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "Engine/DecalActor.h"
#include "AI/Character/Melee/SATORI_Melee.h"
#include "Components/BoxComponent.h"
#include "SATORI_DashMeleeActor.generated.h"

class USphereComponent;
class UBoxComponent;
class UMaterialInterface;
class UDecalComponent;

UCLASS()
class SATORI_API ASATORI_DashMeleeActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASATORI_DashMeleeActor();

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	UDecalComponent* Decal = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	UBoxComponent* CollisionBoxComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Dash")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Dash")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Dash")
	AActor* OwnerMelee;

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

	ASATORI_Melee* MeleeCharacter;

	bool bDamagePlayer = false;
	bool bEndDash = false;

};
