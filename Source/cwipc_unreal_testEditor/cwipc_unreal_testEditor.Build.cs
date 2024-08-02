// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class cwipc_unreal_testEditor : ModuleRules
{
 
    public cwipc_unreal_testEditor(ReadOnlyTargetRules Target) : base(Target)
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
				"cwipc_unreal_testEditor/Public",
                Path.Combine(api_location, "include"),
 			 });
		PrivateIncludePaths.AddRange(
			new string[] {
				"cwipc_unreal_testEditor/Private",
			});
        
		PublicAdditionalLibraries.AddRange(
            new string[] {
                    Path.Combine(api_location, "lib", "cwipc_util.lib"),
            });

			PublicDependencyModuleNames.AddRange(
			new string[] {
				"cwipc_unreal_test",
			});
			
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"UnrealEd"
			});
    }
}
