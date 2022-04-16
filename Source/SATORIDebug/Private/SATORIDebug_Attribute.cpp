#include "SATORIDebug/Public/SATORIDebug_Attribute.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayDebuggerCategory.h"
#include "GameplayDebugger.h"
#include "GameplayDebuggerCategoryReplicator.h"
#include "DebugRenderSceneProxy.h"

SATORIDebug_Attribute::SATORIDebug_Attribute()
{
	bShowOnlyWithDebugActor = false;
	
}

TSharedRef<FGameplayDebuggerCategory> SATORIDebug_Attribute::MakeInstance()
{
	return MakeShareable(new SATORIDebug_Attribute());
}

void SATORIDebug_Attribute::CollectData(class APlayerController* OwnerPC, class AActor* DebugActor)
{
	if (DebugActor)
	{
		UAbilitySystemComponent* AbilityComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DebugActor);
		if (AbilityComp)
		{
			TArray<FGameplayAttribute> Attributes;
			AbilityComp->GetAllAttributes(Attributes);

			for (FGameplayAttribute Attribute : Attributes)
			{
				if (Attribute.IsValid())
				{
					FDebugAttributeData NewData;
					NewData.AttributeName = Attribute.AttributeName;
					NewData.AttributeCurrentValues = AbilityComp->GetNumericAttribute(Attribute);
					AttributeDatas.Add(NewData);
				}
			}
		}
	}
}

void SATORIDebug_Attribute::DrawData(class APlayerController* OwnerPC, class FGameplayDebuggerCanvasContext& CanvasContext)
{
	if (OwnerPC)
	{
		for (FDebugAttributeData Data : AttributeDatas)
		{
			FString DataString = FString::Printf(TEXT("%s : % f"), *Data.AttributeName, Data.AttributeCurrentValues);
			CanvasContext.Print(FColor::Yellow, DataString);
		}
	}
	AttributeDatas.Empty();
}

#endif