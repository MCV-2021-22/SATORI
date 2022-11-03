#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "Components/DecalComponent.h"
#include "SATORI_RaijinRayo.generated.h"

class UNiagaraComponent;
class ASATORICharacter;
class ADecalActor;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UCapsuleComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, Abstract)
class ASATORI_RaijinRayo : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_RaijinRayo();

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* PSC = nullptr;

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
	float Damage = 0.001;

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
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void setDirection(FVector newDirection);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
		TSubclassOf<UGameplayEffect> HitGameplayEffect;

	bool CheckCollision();

	ADecalActor* my_decal = nullptr;


protected:



	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

	float inmunity = 1.f;

	float LifeTime = 8.f;

	

	float time_to_overlap = 0.5;

	//float time_to_destroy = 1.5;
	float time_to_destroy = 0.5;

	float time_actual = 0;

	bool player_inside = true;

	ASATORICharacter* Player = nullptr;

};



