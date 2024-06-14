// This project is made for a test assignment

using UnrealBuildTool;
using System.Collections.Generic;

public class TPSGameTarget : TargetRules
{
	public TPSGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "TPSGame" } );
	}
}
