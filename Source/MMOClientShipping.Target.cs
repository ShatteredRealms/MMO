// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MMOClientShippingTarget : MMOClientTarget 
{
	public MMOClientShippingTarget(TargetInfo Target) : base(Target)
	{
		CustomConfig = "Shipping";
	}
}
