#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "FMODBlueprintStatics.h"
#include "SATORI_ArcherProjectile.generated.h"

class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS(Blueprintable, Abstract)
class ASATORI_ArcherProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_ArcherProjectile();

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* Moving_Projectile = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particle")
		UParticleSystem* Impact_Particle = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float radius = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float velocity = 20.0f;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float damage = 10.0f;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
		UFMODEvent* ProjectileHit = nullptr;

	void DestroySelfByParry();
protected:

	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

	float inmunity = 1.f;

	float LifeTime = 8.f;

	USATORI_GameInstance* GameInstanceRef;
};



