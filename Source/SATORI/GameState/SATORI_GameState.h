// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "GameFramework/GameState.h"
#include "SATORI_GameState.generated.h"

class ASATORI_Portal;
class USATORI_PortalPassiveDataAsset;


UCLASS()
class SATORI_API ASATORI_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASATORI_GameState();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// Portal 
	TArray<ASATORI_Portal*> InstancePortals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portal")
	USATORI_PortalPassiveDataAsset* PassiveDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal")
	TArray<FSATORI_DoorPassiveReward> PortalEffectsToApply;

	//EnemyActors

	//TArray < AActor* >& GetEnemyActorsRef() { return EnemyActors; };

	//UFUNCTION()
	//void AddEnemyActor(AActor* Enemy);

	//UFUNCTION()
	//void RemoveEnemyActor(AActor* Enemy);

	//UFUNCTION()
	//bool IsEmptyEnemyActors();

private:

	//UPROPERTY()
	//TArray < AActor* > EnemyActors;

	int GenerateRandomNumberForPortal();

	void FillPortalGameplayEffectWithData();
};
