#include "SATORIDebug/Public/SATORIDebugModule.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "SATORIDebug/Public/SATORIDebug_Attribute.h"
#include "GameplayDebugger.h"
#endif

DEFINE_LOG_CATEGORY(SATORIDebugModule);

#define LOCTEXT_NAMESPACE "FSATORIDebugModule"

FSATORIDebugModule::FSATORIDebugModule()
{
	UE_LOG(LogTemp, Warning, TEXT("FSATORIDebugModule module has started!"));
}

void FSATORIDebugModule::StartupModule()
{
	UE_LOG(SATORIDebugModule, Warning, TEXT("FSATORIDebugModule module has started!"));
	
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();

	GameplayDebuggerModule.RegisterCategory("Character Attributtes", IGameplayDebugger::FOnGetCategory::CreateStatic(
		&SATORIDebug_Attribute::MakeInstance), EGameplayDebuggerCategoryState::EnabledInSimulate);
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FSATORIDebugModule::ShutdownModule()
{
	UE_LOG(SATORIDebugModule, Warning, TEXT("SATORIDebugModule module has shutdown"));
#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("Character Attributtes");
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSATORIDebugModule, SATORIDebugModule)