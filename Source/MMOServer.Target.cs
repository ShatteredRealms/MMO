// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class MMOServerTarget : TargetRules
{
	public MMOServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange(new string[] { "LyraGame" });

		MMOGameTarget.ApplySharedLyraTargetSettings(this);

		bUseChecksInShipping = true;
	}
}
