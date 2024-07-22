// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;
public class ManiacManfredEditorTarget : TargetRules
{
	public ManiacManfredEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "ManiacManfred" } );
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
    }
}
