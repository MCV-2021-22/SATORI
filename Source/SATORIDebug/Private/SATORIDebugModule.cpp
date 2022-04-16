#include "SATORIDebug/Public/SATORIDebugModule.h"

DEFINE_LOG_CATEGORY(SATORIDebugModule);

#define LOCTEXT_NAMESPACE "FSATORIDebugModule"

FSATORIDebugModule::FSATORIDebugModule()
{
	UE_LOG(LogTemp, Warning, TEXT("FSATORIDebugModule module has started!"));
}

void FSATORIDebugModule::StartupModule()
{
	UE_LOG(SATORIDebugModule, Warning, TEXT("FSATORIDebugModule module has started!"));
	
}

void FSATORIDebugModule::ShutdownModule()
{
	UE_LOG(SATORIDebugModule, Warning, TEXT("SATORIDebugModule module has shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSATORIDebugModule, SATORIDebugModule)