// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class OnlineSubsystemSRO : ModuleRules
{
	public OnlineSubsystemSRO(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDefinitions.Add("ONLINESUBSYSTEMSRO_PACKAGE=1");
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"CommonSRO",
			});

		PrivateDependencyModuleNames.AddRange(
			new string[] { 
				"CommonSRO",
				"Core",
				"CoreOnline",
				"ApplicationCore",
				"HTTP",
				"Json",
				"OnlineSubsystem",
				"Engine",
				"LoginFlow",
				"SROLoginWindow", 
			}
		);

		PrivateIncludePaths.Add("Private/");
	}
}
