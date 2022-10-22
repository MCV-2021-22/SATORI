#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "SATORI_FujinCharm.generated.h"

class ASATORICharacter;
class ASATORI_Fujin;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UBoxComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, Abstract)
class ASATORI_FujinCharm : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_FujinCharm();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* HitAreaComponent = nullptr;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particle")
	UNiagaraSystem* Charm_Hit_Particle = nullptr;

	bool ReturnToFujin = false;

	bool ReturnToFujinDone = false;

	bool canDestroy = false;

	ASATORI_Fujin* Fujin = nullptr;

protected:

	ASATORICharacter* Player = nullptr;
	

	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

	float inmunity = 1.f;

	float LifeTime = 8.f;

	USATORI_GameInstance* GameInstanceRef;


	

};



