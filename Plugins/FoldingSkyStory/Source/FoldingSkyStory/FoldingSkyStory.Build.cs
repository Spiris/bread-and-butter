// Copyright Folding Sky Games LLC 2021 All rights reserved.

using UnrealBuildTool;

public class FoldingSkyStory : ModuleRules
{
	public FoldingSkyStory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"Json",
				"JsonUtilities"
			});
	}
}
