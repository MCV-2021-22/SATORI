// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_Fujin.generated.h"


class ASATORI_Raijin;
class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_Fujin : public ASATORI_AICharacter
{
	GENERATED_BODY()

public:
	ASATORI_Fujin();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	bool moveBackwards = false;
	bool firstmoveBackwards = true;

	bool canMove = true;

	bool moveRight = false;

	bool moveLeft = false;

	float getDistAttack();

	float getCloseDist();

	ASATORI_Raijin* Raijin;

	FTimerHandle TimerHandle;

	FTimerHandle TimerHandleDowned;

	void setRaijin();

	UFUNCTION(BlueprintType, BlueprintCallable)
		bool getDowned();

	UFUNCTION(BlueprintType, BlueprintCallable)
		bool getDead();

	void setDowned(bool dw);

	void setDead(bool dw);

	void startCDDowned();

	void revivirTag();

	void revivir();

	bool getRaijinDowned();


	FVector posinicial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UCapsuleComponent* CollisionL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UCapsuleComponent* CollisionR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UNiagaraComponent* Nube1;

	UPROPERTY(EditDefaultsOnly)
		UNiagaraSystem* Nube = nullptr;

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


	TArray< float > GetArrayProbs(int Combo);

protected:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
		USkeletalMeshComponent* Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
		USkeletalMeshComponent* Right;

	float dist_attack = 800.0f;

	float close_dist = 100.0f;

	bool downed = false;

	bool dead = false;

	float CdTimeDowned = 10.0f;

	TArray< float > Prob_C1;
	TArray< float > Prob_C2;
	TArray< float > Prob_C3;
	TArray< float > Prob_C4;
	


	
};



