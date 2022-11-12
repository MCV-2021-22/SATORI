// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_Fujin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIFujinHealthChanges, bool, isChanges);

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

	// Health Notify
	FSATORIFujinHealthChanges BossHealthChanges;
	void BossHealthNotifyAbilityChanged();

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

	bool getGolpeFuerte();

	void setGolpeFuerte(bool golpe);

	void ActivarInputPlayer();

	bool hit_r = false;
	bool hit_l = false;

	bool hit_heavy_r = false;
	bool hit_heavy_l = false;

	bool player_inside_l = false;

	bool player_inside_r = false;

	FVector posinicial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UCapsuleComponent* CollisionL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UCapsuleComponent* CollisionR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UNiagaraComponent* Nube1;

	UPROPERTY(EditDefaultsOnly)
		UNiagaraSystem* Nube = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
		TSubclassOf<UGameplayEffect> HitGameplayEffect1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
		TSubclassOf<UGameplayEffect> HitHeavyGameplayEffect1;


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

	void SetArrayProbs(int Combo, TArray<float> newArray);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		float dmg_right_low = 0.001;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		float dmg_right_huge = 0.001;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		float dmg_left_low = 0.001;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		float dmg_left_huge = 0.001;


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

	bool golpe_fuerte = false;

	ASATORICharacter* Player = nullptr;

	TArray< float > Prob_C1;
	TArray< float > Prob_C2;
	TArray< float > Prob_C3;
	TArray< float > Prob_C4;
	


	
};



