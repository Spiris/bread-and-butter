// Copyright Folding Sky Games LLC 2021 All rights reserved.

using UnrealBuildTool;

public class BNB : ModuleRules
{
	public BNB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemNull",
            "Networking",
            "Sockets",

             "FoldingSkyStory",
        });
    }
}
