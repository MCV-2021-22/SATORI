// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SATORI_PortalGrantedAbilityAsset.generated.h"

class UTexture2D;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FPortalGrantedAbilityDatas
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> PortalGrantedAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText PortalGrantedAbilityName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* PortalGrantedAbilitiyIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool isUpgrated = false;
};

UCLASS()
class SATORI_API USATORI_PortalGrantedAbilityAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	USATORI_PortalGrantedAbilityAsset();

	// Array of abilities
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FPortalGrantedAbilityDatas> GrantedAbilities;
};
