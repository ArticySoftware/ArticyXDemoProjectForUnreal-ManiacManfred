// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;
public class ManiacManfredEditorTarget : TargetRules
{
	public ManiacManfredEditorTarget(TargetInfo target) : base(target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.Add("ManiacManfred");
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
    }
}
