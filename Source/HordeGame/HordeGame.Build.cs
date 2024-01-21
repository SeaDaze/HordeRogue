// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HordeGame : ModuleRules
{
	public HordeGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore", "UMG" });
	}
}
