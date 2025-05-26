// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MMOEditorTarget : TargetRules
{
	public MMOEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange(new string[] { "LyraGame", "LyraEditor" });

		if (!bBuildAllModules)
		{
			NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
		}

		MMOGameTarget.ApplySharedLyraTargetSettings(this);

		// This is used for touch screen development along with the "Unreal Remote 2" app
		EnablePlugins.Add("RemoteSession");
	}
}
