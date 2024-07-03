// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class cwipc_unreal_test : ModuleRules
{
	public cwipc_unreal_test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
