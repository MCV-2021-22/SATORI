#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameplayDebuggerCategory.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayDebuggerCategory.h"

class SATORIDebug_Attribute : public FGameplayDebuggerCategory
{
public:
	SATORIDebug_Attribute();

	/** Creates an instance of this category - will be used on module startup to include our category in the Editor */
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	/** Collects the data we would like to print */
	virtual void CollectData(class APlayerController* OwnerPC, class AActor* DebugActor) override;

	/** Displays the data we collected in the CollectData function */
	virtual void DrawData(class APlayerController* OwnerPC, class FGameplayDebuggerCanvasContext& CanvasContext) override;

protected:
	//The data we're going to print inside the viewport
	struct FDebugAttributeData
	{
		FString AttributeName;
		float AttributeCurrentValues;
	};

	TArray<FDebugAttributeData> AttributeDatas;
};

#endif