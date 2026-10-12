// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// EclipseCore: engine-agnostic rules (C++17, STL only). Depends on "Core" solely
// for IMPLEMENT_MODULE; no gameplay file includes engine headers, which keeps the
// module compilable by the standalone test build in /Tests.

using UnrealBuildTool;

public class EclipseCore : ModuleRules
{
	public EclipseCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;
		bEnableExceptions = false;

		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
	}
}
