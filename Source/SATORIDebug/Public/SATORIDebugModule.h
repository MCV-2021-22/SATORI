#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// https://www.orfeasel.com/creating-custom-gameplay-debugger-categories/
// http://www.orfeasel.com/creating-custom-modules/

DECLARE_LOG_CATEGORY_EXTERN(SATORIDebugModule, All, All);

class FSATORIDebugModule : public FDefaultGameModuleImpl
{
	/* This will get called when the editor loads the module */
	virtual void StartupModule() override;

	/* This will get called when the editor unloads the module */
	virtual void ShutdownModule() override;

public:
	FSATORIDebugModule();
};