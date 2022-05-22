// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SATORI_BlackHoleActor.generated.h"

class USphereComponent;

UCLASS()
class SATORI_API ASATORI_BlackHoleActor : public AActor
{
	GENERATED_BODY()

	APlayerController* Controller;
	TArray<UPrimitiveComponent*> ArrayTrapped;
	bool Exploded = false;

	FTimerHandle UnusedHandle;

public:	
	// Sets default values for this actor's properties
	ASATORI_BlackHoleActor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SphereRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float TimeToDestroy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool Active = false;

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SphereRadiusOnExplosion;

	UFUNCTION(BlueprintCallable)
	void Explode();

	UFUNCTION()
		void OnOverlapSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapSphereOnExplosion(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	void OnTimerExpiredDestroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
