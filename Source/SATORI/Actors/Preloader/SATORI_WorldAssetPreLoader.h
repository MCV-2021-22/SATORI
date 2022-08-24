// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SATORI_WorldAssetPreLoader.generated.h"

class USATORI_AbilityDataAsset;

UCLASS()
class SATORI_API ASATORI_WorldAssetPreLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_WorldAssetPreLoader();

	// Default abilities assets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	USATORI_AbilityDataAsset* DefaultAbilities;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Apply Abilities
	void ApplyDefaultPlayerGameplayAbilities();
};
