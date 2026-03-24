

using System.IO;
using UnrealBuildTool;

public class LSWebpLibrary : ModuleRules
{
    public LSWebpLibrary(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;


        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
           

            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "libwebp.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "libwebpdemux.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "libwebpmux.lib"));

        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {

        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {

        }
    }
}
