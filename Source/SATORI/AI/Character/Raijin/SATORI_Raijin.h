// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Components/Raijin/SATORI_ArcoAltavoces.h"
#include "SATORI_Raijin.generated.h"

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

	UFUNCTION(BlueprintCallable)
	FVector getSpawnPos();

	void setSpawnPos(FVector Pos);

	ASATORI_ArcoAltavoces* ArcoAltavoces;

	ASATORI_Fujin* Fujin;

	FTimerHandle TimerHandle;

	FTimerHandle TimerHandleFujin;

	FTimerHandle TimerHandleDowned;

	FTimerDelegate TimerDelegate;

	void setArcoAltavoces();

	void setFujin();

	bool getDowned();

	void setDowned(bool dw);

	void startCDDowned();

	void revivir();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UStaticMeshComponent* SwordComponentRight;

protected:

	FVector SpawnPos;

	float SoCloseDist = 250.0f;

	bool downed = false;

	float CdTimeDowned = 10.0f;

};
