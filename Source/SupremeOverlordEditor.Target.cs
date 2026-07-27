using UnrealBuildTool;
using System.Collections.Generic;

public class SupremeOverlordEditorTarget : TargetRules
{
	public SupremeOverlordEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("SupremeOverlord");
	}
}
