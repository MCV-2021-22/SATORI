// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SATORIDebug : ModuleRules
{
	public SATORIDebug(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",
			"GameplayAbilities", "GameplayTags", "GameplayTasks",});

		//The path for the header files
		PublicIncludePaths.AddRange(new string[] { "SATORIDebug/Public" });

		//The path for the source files
		PrivateIncludePaths.AddRange(new string[] { "SATORIDebug/Private" });

		PublicIncludePaths.Add("SATORIDebug");
	}
}
