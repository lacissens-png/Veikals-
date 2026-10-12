// Copyright Eclipse of the Primordials. All Rights Reserved.

using UnrealBuildTool;

public class EclipseGame : ModuleRules
{
	public EclipseGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"DeveloperSettings",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"NetCore",
			"EclipseCore"
		});
	}
}
