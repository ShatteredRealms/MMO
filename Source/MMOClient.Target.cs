// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MMOClientTarget : TargetRules
{
	public MMOClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

		ExtraModuleNames.AddRange(new string[] { "LyraGame" });

		MMOGameTarget.ApplySharedLyraTargetSettings(this);
	}
}
