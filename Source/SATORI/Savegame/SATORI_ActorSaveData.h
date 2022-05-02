#pragma once

#include "CoreMinimal.h"
#include "SATORI_ActorSaveData.generated.h"

USTRUCT(BlueprintType)
struct FSATORI_ActorSaveRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FName> ActorName;

	UPROPERTY(BlueprintReadWrite)
	FTransform ActorTransform;

	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> SerializedActorData;
};

USTRUCT(BlueprintType)
struct FSATORI_ActorDeleteData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FName> DeleteActorName;
};

USTRUCT(BlueprintType)
struct FSATORI_ActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FSATORI_ActorDeleteData> DeletedActorsByLevel;
};
