// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SATORI_PortalPassiveDataAsset.generated.h"

class UGameplayEffect;
class UTexture2D;

USTRUCT(BlueprintType)
struct FSATORI_DoorPassiveDatas
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> PassiveEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* PassiveIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
};

UCLASS()
class SATORI_API USATORI_PortalPassiveDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	USATORI_PortalPassiveDataAsset();

	// Array of abilities
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FSATORI_DoorPassiveDatas> PassiveRewards;
};
