// Copyright Eclipse of the Primordials. All Rights Reserved.

using UnrealBuildTool;

public class EclipseOfThePrimordialsEditorTarget : TargetRules
{
	public EclipseOfThePrimordialsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange(new string[] { "EclipseCore", "EclipseGame" });
	}
}
