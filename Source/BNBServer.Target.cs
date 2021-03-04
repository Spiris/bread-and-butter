// Copyright Folding Sky Games LLC 2021 All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BNBServerTarget : TargetRules
{
	public BNBServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "BNB" } );
	}
    //public override void SetupGlobalEnvironment(
    //        TargetInfo Target,
    //        ref LinkEnvironmentConfiguration OutLinkEnvironmentConfiguration,
    //        ref CPPEnvironmentConfiguration OutCPPEnvironmentConfiguration
    //        )
    //{        
    //    bCompileLeanAndMeanUE = true;
    //    bBuildEditor = false;
    //    bBuildWithEditorOnlyData = false;
    //    bBuildRequiresCookedData = true;
    //    bCompileAgainstEngine = true;
    //    bWithPerfCounters = true;
    //    bUseLoggingInShipping = true;
    //
    //    // Tag it as a 'Server' build
    //    OutCPPEnvironmentConfiguration.Definitions.Add("UE_SERVER=1");
    //    OutCPPEnvironmentConfiguration.Definitions.Add("USE_NULL_RHI=1");
    //    OutCPPEnvironmentConfiguration.Definitions.Add("UE_GAME=0");
    //    OutCPPEnvironmentConfiguration.Definitions.Add("USE_LOGGING_IN_SHIPPING=1");
    //    
    //    // no exports, so no need to verify that a .lib and .exp file was emitted by the linker.
    //    OutLinkEnvironmentConfiguration.bHasExports = false;
    //}
}
