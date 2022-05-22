// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "SATORI_MisileActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SATORI_API ASATORI_MisileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_MisileActor();

	UPROPERTY(EditDefaultsOnly, Category = "Misile")
	USphereComponent* CollisionSphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Misile")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Misile")
	USphereComponent* SeekingSphereComponent;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Misile")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Misile")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Misile")
	float TimeToDestroy;

	UFUNCTION(BlueprintCallable, Category = "Misile")
		void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Misile")
		void OnOverlapSeekingSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:

	AActor* Target;

	FTimerHandle TimerHandleDestroy;

	void Explode();

};
