#pragma once

#include "CoreMinimal.h"

#include "GameplayTagAssetInterface.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "SATORI_FujinLevantarCollision.generated.h"

class ASATORICharacter;
class ASATORI_Fujin;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;

UCLASS(Blueprintable, Abstract)
class ASATORI_FujinLevantarCollision : public AActor
{
	GENERATED_BODY()

public:
	ASATORI_FujinLevantarCollision();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		float TraceDistanceToFloor = 250.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		float HeightChange = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		float MaxHeight = 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		float MinHeight = 30.0f;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;


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

	void InmunityOff();

	void ApplyDmg(ASATORICharacter* Player);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	

	FTimerHandle TimerHandle;


	void PushPlayer(ASATORICharacter* Player);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		float ImpulseForce;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		float Damage = 0.001;


protected:

	ASATORICharacter* Player = nullptr;
	

	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;

	FVector direction;

	float inmunityTime = 2.f;

	bool inmunity = true;

	float LifeTime = 2.5f;

	USATORI_GameInstance* GameInstanceRef;


	

};



