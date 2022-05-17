
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTags.h"
#include "SATORI_GameplayTagInterface.generated.h"

/** Interface for assets which contain gameplay tags */
UINTERFACE(Blueprintable, meta=(CannotImplementInterfaceInBlueprint))
class USATORI_GameplayTagInterface : public UInterface
{
	GENERATED_BODY()
};

class SATORI_API ISATORI_GameplayTagInterface
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = SATORIGameplayTags)
	virtual void AddGameplayTag(const FGameplayTag& TagToAdd) = 0;

	UFUNCTION(BlueprintCallable, Category= SATORIGameplayTags)
	virtual void RemoveGameplayTag(const FGameplayTag& TagToRemove) = 0;

};

