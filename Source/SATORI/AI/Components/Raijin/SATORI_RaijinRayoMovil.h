#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "Components/DecalComponent.h"
#include "SATORI_RaijinRayoMovil.generated.h"

class ASATORICharacter;
class ADecalActor;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UCapsuleComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, Abstract)
class ASATORI_RaijinRayoMovil : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_RaijinRayoMovil();

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* CapsuleComponentFinal = nullptr;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float velocity = 10.0f;

	bool inmune = true;
	float timeiNMUNE = 1.0F;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	float Damage = 10;

	UFUNCTION()
		void OnComponentBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentBeginOverlapFinal(
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

	void destroyDecal();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	bool getDestruido();

protected:



	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

	float inmunity = 1.f;

	float LifeTime = 8.f;

	ADecalActor* my_decal = nullptr;


	float time_to_destroy = 8.0f;

	float decal_size = 300.0f;

	float scale = 1.0f;

	bool destruido = false;

};



