// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Portal/SATORI_Portal.h"
#include "GameFramework/GameState.h"
#include "SATORI_GameState.generated.h"

class ASATORI_Portal;
class USATORI_PortalPassiveDataAsset;
class USATORI_PortalGrantedAbilityAsset;
class USATORI_GameInstance;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portal")
	USATORI_PortalGrantedAbilityAsset* GrantedAbilityDataAsset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Portal")
	TArray<FSATORI_DoorPassiveReward> PortalEffectsToApply;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Portal")
	TArray<FSATORI_PortalAbilitiesDatasReward> PortalGrantedAbilityToApply;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Portal")
	TArray<FSATORI_PortalAbilitiesDatasReward> PortalGrantedUpgratedAbilityToApply;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Portal")
	TArray<FSATORI_PortalAbilitiesDatasReward> PortalGrantedNormalAbilityToApply;

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

	// Fill datas assets
	void FillPortalGameplayEffectWithData();
	void FillPortalGrantedAbilityWithData();

	// Generated Random Effect o Granted Random Ability
	void GeneratedRandomPassiveEffect();
	void GeneratedRandomPlayerAbility();

	void GenerateRandomPassiveEffectAndAbilities();

	USATORI_GameInstance* GameInstanceRef = nullptr;
};
