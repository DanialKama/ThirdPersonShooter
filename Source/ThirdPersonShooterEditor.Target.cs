// Copyright 2022 Danial Kamali. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ThirdPersonShooterEditorTarget : TargetRules
{
	public ThirdPersonShooterEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
		ExtraModuleNames.AddRange( new string[] { "ThirdPersonShooter" } );
	}
}
