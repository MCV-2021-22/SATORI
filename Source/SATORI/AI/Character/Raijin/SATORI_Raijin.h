// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Components/Raijin/SATORI_ArcoAltavoces.h"
#include "SATORI_Raijin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIRaijinHealthChanges, bool, isChanges);

class UNiagaraComponent;
class UNiagaraSystem;
class ASATORI_Fujin;
/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_Raijin : public ASATORI_AICharacter
{
	GENERATED_BODY()

public:
	ASATORI_Raijin();

	virtual void BeginPlay() override;

	float getSoCloseDist();

	float getSoCloseDistExt();

	UFUNCTION(BlueprintCallable)
	FVector getSpawnPos();

	void setSpawnPos(FVector Pos);

	UPROPERTY(EditDefaultsOnly)
		UNiagaraSystem* ArcoPart = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UNiagaraComponent* ParticleEffectArco = nullptr;

	// Health Notify
	FSATORIRaijinHealthChanges BossHealthChanges;
	void BossHealthNotifyAbilityChanged();

	ASATORI_ArcoAltavoces* ArcoAltavoces;

	ASATORI_Fujin* Fujin;

	FTimerHandle TimerHandle;

	FTimerHandle TimerHandleFujin;

	FTimerHandle TimerHandleDowned;

	FTimerDelegate TimerDelegate;

	void setArcoAltavoces();

	void setFujin();

	UFUNCTION(BlueprintType, BlueprintCallable)
		bool getDowned();

	UFUNCTION(BlueprintType, BlueprintCallable)
		bool getDead();

	void setDowned(bool dw);

	void setDead(bool dw);

	void startCDDowned();

	void revivirTag();

	void revivir();

	bool getFujinDowned();

	void setNumRayosExtensibles(int num);

	int getNumRayosExtensibles();

	void StartPartArco();

	void EndPartArco();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UStaticMeshComponent* SwordComponentRight;

protected:

	FVector SpawnPos;

	float SoCloseDist = 400.0f;

	float SoCloseDistExt = 1750.0f;

	bool downed = false;

	bool dead = false;

	float CdTimeDowned = 10.0f;

	int num_rayos_extensibles = 3;

};
