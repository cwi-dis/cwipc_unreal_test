// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class cwipc_unreal_testEditor : ModuleRules
{
    const string CWIPC_API_LOCATION = "C:/Program Files/cwipc 7.5.3/";

    public cwipc_unreal_testEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(
			 new string[] {
				"cwipc_unreal_testEditor/Public",
				Path.Combine(CWIPC_API_LOCATION, "include"),
			 });
		PrivateIncludePaths.AddRange(
			new string[] {
				"cwipc_unreal_testEditor/Private",
			});
        PublicAdditionalLibraries.AddRange(
            new string[] {
                    Path.Combine(CWIPC_API_LOCATION, "lib", "cwipc_util.lib"),
            });
		PublicDependencyModuleNames.AddRange(
			new string[] {
				"cwipc_unreal_test",
			});
    }
}
