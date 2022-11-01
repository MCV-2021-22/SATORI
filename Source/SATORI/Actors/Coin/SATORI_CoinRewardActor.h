// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "FMODBlueprintStatics.h"
#include "SATORI_CoinRewardActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UGameplayEffect;
class UNiagaraSystem;

UCLASS()
class SATORI_API ASATORI_CoinRewardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_CoinRewardActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> CoinRewardEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rotation")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator RotationRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Particle Effect")
	UNiagaraSystem* NS_ParticleEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* EventTakeCoin = nullptr;

	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnCoinDestroy();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FActiveGameplayEffectHandle ActiveGEHandle;
};
