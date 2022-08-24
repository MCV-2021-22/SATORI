// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SATORI/SATORI.h"
#include "SATORI_AbilityDataAsset.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FSATORIGameplayAbilityInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<ESATORIAbilityInputID::Type> AbilityKeys;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UGameplayAbility> SATORIAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText AbilityName;
};

/*
* Data Asset register all player Abilities and we can activating in SATORICharacter Class
*/
UCLASS(BlueprintType)
class SATORI_API USATORI_AbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	USATORI_AbilityDataAsset();

	// Array of abilities
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FSATORIGameplayAbilityInfo> Abilities;

	void GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const;
};
