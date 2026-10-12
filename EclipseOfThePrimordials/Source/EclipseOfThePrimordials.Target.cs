// Copyright Eclipse of the Primordials. All Rights Reserved.

using UnrealBuildTool;

public class EclipseOfThePrimordialsTarget : TargetRules
{
	public EclipseOfThePrimordialsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange(new string[] { "EclipseCore", "EclipseGame" });
	}
}
