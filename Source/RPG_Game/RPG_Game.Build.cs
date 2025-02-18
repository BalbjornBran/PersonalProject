// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RPG_Game : ModuleRules
{
	public RPG_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTasks", "GameplayAbilities", "MetasoundEngine", "MotionCore", "GameplayTags"});

		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Public")
			}
		);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Private")
			}
		);
	}
}
