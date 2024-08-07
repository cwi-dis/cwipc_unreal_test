// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class cwipc_unreal_test : ModuleRules
{

    public cwipc_unreal_test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		string api_location;
		string[] dirs = Directory.GetDirectories("C:/Program Files", "cwipc*");
		if (dirs.Length == 1) {
			api_location = dirs[0];
		} else {
			throw new System.Exception("Cannot find cwipc install directory");
		}

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(
                new string[] {
                    Path.Combine(api_location, "include"),
                 });

        PublicAdditionalLibraries.AddRange(
            new string[] {
                    Path.Combine(api_location, "lib", "cwipc_util.lib"),
                    Path.Combine(api_location, "lib", "cwipc_kinect.lib"),
                    Path.Combine(api_location, "lib", "cwipc_codec.lib"),
                    Path.Combine(api_location, "lib", "cwipc_realsense2.lib"),
            });
    }
}
