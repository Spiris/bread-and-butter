// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class BNBServerTarget : TargetRules
{
	public BNBServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange( new string[] { "BNB" } );
        
	}
    public override void SetupGlobalEnvironment(
            TargetInfo Target,
            ref LinkEnvironmentConfiguration OutLinkEnvironmentConfiguration,
            ref CPPEnvironmentConfiguration OutCPPEnvironmentConfiguration
            )
    {
        UEBuildConfiguration.bCompileLeanAndMeanUE = true;
        UEBuildConfiguration.bBuildEditor = false;
        UEBuildConfiguration.bBuildWithEditorOnlyData = false;
        UEBuildConfiguration.bBuildRequiresCookedData = true;
        UEBuildConfiguration.bCompileAgainstEngine = true;
        UEBuildConfiguration.bWithPerfCounters = true;

        // Tag it as a 'Server' build
        OutCPPEnvironmentConfiguration.Definitions.Add("UE_SERVER=1");
        OutCPPEnvironmentConfiguration.Definitions.Add("USE_NULL_RHI=1");
        OutCPPEnvironmentConfiguration.Definitions.Add("UE_GAME=0");
        OutCPPEnvironmentConfiguration.Definitions.Add("USE_LOGGING_IN_SHIPPING=1");
        UEBuildConfiguration.bUseLoggingInShipping = true;
        // no exports, so no need to verify that a .lib and .exp file was emitted by the linker.
        OutLinkEnvironmentConfiguration.bHasExports = false;
    }
}
