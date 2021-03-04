// Copyright Folding Sky Games LLC 2021 All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BNBEditorTarget : TargetRules
{
	public BNBEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "BNB" } );
	}
}
