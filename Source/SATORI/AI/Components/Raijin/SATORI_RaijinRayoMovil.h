#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"

#include "Components/DecalComponent.h"
#include "SATORI_RaijinRayoMovil.generated.h"

class UNiagaraComponent;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* CapsuleComponentFinal = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UNiagaraSystem* Trueno = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UNiagaraComponent* ParticleEffect = nullptr;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
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
		void OnComponentEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);


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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
		TSubclassOf<UGameplayEffect> HitGameplayEffect;

	bool getDestruido();

protected:



	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

	float inmunity = 1.f;

	float LifeTime = 4.f;

	ADecalActor* my_decal = nullptr;


	float time_to_destroy = 8.0f;

	float decal_size = 300.0f;

	float scale = 1.0f;

	bool destruido = false;

	bool player_inside = true;

	ASATORICharacter* Player = nullptr;

	float time_to_dmg = 0.5f;

	float time_to_dmg_max = 0.5f;
};



