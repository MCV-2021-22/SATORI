#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameplayDebuggerCategory.h"

class SATORIDebug_OwnedTags : public FGameplayDebuggerCategory
{
public:
	SATORIDebug_OwnedTags();

	/** Creates an instance of this category - will be used on module startup to include our category in the Editor */
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	/** Collects the data we would like to print */
	virtual void CollectData(class APlayerController* OwnerPC, class AActor* DebugActor) override;

	/** Displays the data we collected in the CollectData function */
	virtual void DrawData(class APlayerController* OwnerPC, class FGameplayDebuggerCanvasContext& CanvasContext) override;

protected:
	TArray<FString> DebugOwnedTags;
};

#endif