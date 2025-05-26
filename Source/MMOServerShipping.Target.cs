// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class MMOServerShippingTarget : MMOServerTarget
{
	public MMOServerShippingTarget(TargetInfo Target) : base(Target)
	{
		CustomConfig = "Shipping";
	}
}
