
using UnrealBuildTool;

public class LSWebp : ModuleRules
{
    public LSWebp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableUndefinedIdentifierWarnings = false;

        PrivateIncludePaths.AddRange(
            new string[] {
                "LSWebp/Public/Core",
                "LSWebp/Public/Log",
                "LSWebp/Public/Subsystem",
                "LSWebp/Public/Type",
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Engine"

            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",

                "RenderCore",
                "LSWebpLibrary",
                "Projects"

            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );
    }
}
