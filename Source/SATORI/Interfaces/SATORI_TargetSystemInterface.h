#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SATORI_TargetSystemInterface.generated.h"

UINTERFACE(Blueprintable)
class USATORI_TargetSystemInterface : public UInterface
{
	GENERATED_BODY()
};

class SATORI_API ISATORI_TargetSystemInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	bool IsTargetable() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	void RegisterInTargetableArray();
};
