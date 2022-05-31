#include "SATORIDebug/Public/SATORIDebug_OwnedTags.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayDebuggerCategory.h"
#include "GameplayDebugger.h"
#include "GameplayDebuggerCategoryReplicator.h"
#include "DebugRenderSceneProxy.h"
#include "GameplayTagContainer.h"

SATORIDebug_OwnedTags::SATORIDebug_OwnedTags()
{
	bShowOnlyWithDebugActor = false;
}

TSharedRef<FGameplayDebuggerCategory> SATORIDebug_OwnedTags::MakeInstance()
{
	return MakeShareable(new SATORIDebug_OwnedTags());
}

void SATORIDebug_OwnedTags::CollectData(class APlayerController* OwnerPC, class AActor* DebugActor)
{
	if (DebugActor)
	{
		UAbilitySystemComponent* AbilityComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DebugActor);
		if (AbilityComp)
		{
			DebugOwnedTags.Reset();
			static FGameplayTagContainer OwnerTags;
			AbilityComp->GetOwnedGameplayTags(OwnerTags);
			for (const FGameplayTag& Tag : OwnerTags)
			{
				DebugOwnedTags.AddUnique(Tag.ToString());
			}
		}
	}
}

void SATORIDebug_OwnedTags::DrawData(class APlayerController* OwnerPC, class FGameplayDebuggerCanvasContext& CanvasContext)
{

	if (OwnerPC)
	{
		for (FString Tags : DebugOwnedTags)
		{
			CanvasContext.Print(FColor::Yellow, Tags);
		}
	}

}

#endif