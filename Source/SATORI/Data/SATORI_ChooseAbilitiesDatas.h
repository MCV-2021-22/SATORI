// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SATORI_ChooseAbilitiesDatas.generated.h"

class UTexture2D;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilitiesChooseDatas
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> SATORIAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText AbilityName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* AbilitiyIcon;
};

UCLASS()
class SATORI_API USATORI_ChooseAbilitiesDatas : public UDataAsset
{
	GENERATED_BODY()
	
public:

	USATORI_ChooseAbilitiesDatas();

	// Array of abilities
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FAbilitiesChooseDatas> Abilities;
};
