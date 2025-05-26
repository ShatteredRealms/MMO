// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModularWindows : ModuleRules
{
	public ModularWindows(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;				
	
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"InputCore",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"CommonInput",
				"CommonUI",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
