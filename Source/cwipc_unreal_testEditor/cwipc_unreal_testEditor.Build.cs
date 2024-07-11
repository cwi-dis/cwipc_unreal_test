// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class cwipc_unreal_test : ModuleRules
{
    const string CWIPC_API_LOCATION = "C:/Program Files/cwipc 7.5.3/";

    public cwipc_unreal_test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(
                new string[] {
                    Path.Combine(CWIPC_API_LOCATION, "include"),
                 });

        PublicAdditionalLibraries.AddRange(
            new string[] {
                    Path.Combine(CWIPC_API_LOCATION, "lib", "cwipc_util.lib"),
            });
    }
}
