// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MacroSystem : ModuleRules
{
	public MacroSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrecompileForTargets = PrecompileTargetsType.Any;
		bUsePrecompiled = true;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"StructUtils",
				"DeveloperSettings",
				"UMG"
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);
	}
}
