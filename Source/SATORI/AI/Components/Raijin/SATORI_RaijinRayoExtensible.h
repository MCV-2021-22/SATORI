#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "Components/DecalComponent.h"
#include "SATORI_RaijinRayoExtensible.generated.h"

class ASATORICharacter;
class ADecalActor;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UCapsuleComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, Abstract)
class ASATORI_RaijinRayoExtensible : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_RaijinRayoExtensible();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USphereComponent* SphereComponent = nullptr;


	UPROPERTY(EditDefaultsOnly)
		UNiagaraSystem* Trueno = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
		UDecalComponent* Decal = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UMaterialInterface* MaterialDecal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float Damage = 20;

	

	UFUNCTION()
		void OnComponentBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void setDirection(FVector newDirection);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
		TSubclassOf<UGameplayEffect> HitGameplayEffect;


	bool destruible = false;


protected:



	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

	float inmunity = 1.f;

	float LifeTime = 8.f;

	ADecalActor* my_decal = nullptr;


	float time_to_destroy = 5.0f;

	float decal_size = 300.0f;

	float scale = 1.0f;

};



