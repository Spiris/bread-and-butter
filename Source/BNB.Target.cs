// Copyright Folding Sky Games LLC 2021 All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BNBTarget : TargetRules
{
	public BNBTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "BNB" } );
	}
}
