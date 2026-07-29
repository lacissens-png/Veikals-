using UnrealBuildTool;
using System.Collections.Generic;

public class SupremeOverlordTarget : TargetRules
{
	public SupremeOverlordTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("SupremeOverlord");
	}
}
