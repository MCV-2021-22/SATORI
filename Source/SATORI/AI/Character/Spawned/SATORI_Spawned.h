// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/Spawner/SATORI_Spawner.h"
#include "FMODBlueprintStatics.h"
#include "SATORI_Spawned.generated.h"

class UNiagaraSystem;

UCLASS()
class SATORI_API ASATORI_Spawned : public ASATORI_AICharacter
{
	GENERATED_BODY()

public:

	ASATORI_Spawned();

	void SetMySpawn(ASATORI_Spawner* SpawnToSet);

	virtual void PossessedBy(AController* NewController) override;

	float GetDistAttack() const;

	void SpawnedDie();

	ASATORI_Spawner* MySpawn = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Particle Effect")
	UNiagaraSystem* NS_ParticleEffect = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Particle Effect")
	UNiagaraSystem* NS_ParticleEffect2 = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* SpawnedDeath = nullptr;

protected:

	float DistAttack = 700.f;
};
