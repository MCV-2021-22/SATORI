// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SATORI : ModuleRules
{
	public SATORI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",

			"GameplayAbilities", "GameplayTags", "GameplayTasks","AIModule","Niagara","NavigationSystem", "MoviePlayer", "FMODStudio"});


		PublicIncludePaths.Add("SATORI");
	
	}
}
