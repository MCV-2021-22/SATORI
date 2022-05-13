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

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SphereRadius;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float TimeToDestroy;

	ASATORI_PushSphere();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent = nullptr;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	void InitializeParameters(float, float, float);

	void OnTimerExpiredDestroy();

private:

	TArray<UPrimitiveComponent*> ArrayPushed;

};
