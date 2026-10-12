// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Dedicated server target. The server owns all loot, forge, swarm simulation and
// damage; it never renders, so the swarm subsystem skips ISM updates here.

using UnrealBuildTool;

public class EclipseOfThePrimordialsServerTarget : TargetRules
{
	public EclipseOfThePrimordialsServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange(new string[] { "EclipseCore", "EclipseGame" });
	}
}
