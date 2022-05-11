// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "SATORI_PushSphere.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class SATORI_API ASATORI_PushSphere : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ASATORI_PushSphere();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SphereRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Power;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Grounded;

	UFUNCTION(BlueprintNativeEvent)
	void HandleHit(AActor* OtherActor);

public:	

	UFUNCTION()
		void OnOverlapSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	virtual void Tick(float DeltaTime) override;


//TO DO: Improve destroy of actor
public:

	void delayedDestroy(float);

private:

	void DestroyMyself();

	UPrimitiveComponent* RootComp;
	
};
