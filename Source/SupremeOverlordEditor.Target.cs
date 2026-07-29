using UnrealBuildTool;
using System.Collections.Generic;

public class SupremeOverlordEditorTarget : TargetRules
{
	public SupremeOverlordEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("SupremeOverlord");
	}
}
