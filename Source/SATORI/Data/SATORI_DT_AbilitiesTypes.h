// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SATORI_DT_AbilitiesTypes.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FAbilitesIconDatas : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText AbilityName;
};
