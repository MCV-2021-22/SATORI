// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_RangeMovable.generated.h"


class USATORI_ArcherProjectileSpawnerComponent;
/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_RangeMovable : public ASATORI_AICharacter
{
	GENERATED_BODY()
	
public:
	
	ASATORI_RangeMovable();
	virtual void Tick(float DeltaTime) override;

	bool moveBackwards = false;
	bool firstmoveBackwards = true;

	bool canMove = true;

	bool moveRight = false;

	bool moveLeft = false;

	float getDistAttack();

	float getCloseDist();

	FVector posinicial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UCapsuleComponent* CollisionB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UCapsuleComponent* CollisionL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UCapsuleComponent* CollisionR;

	UFUNCTION()
		void OnOverlapBehind(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapLeft(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapRight(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapBehind(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);
	UFUNCTION()
		void OnEndOverlapLeft(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);
	UFUNCTION()
		void OnEndOverlapRight(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);




protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
		USkeletalMeshComponent* Back;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
		USkeletalMeshComponent* Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
		USkeletalMeshComponent* Right;

	float dist_attack = 800.0f;

	float close_dist = 100.0f;

};
