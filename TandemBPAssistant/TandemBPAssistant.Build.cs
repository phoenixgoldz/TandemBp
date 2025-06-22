using UnrealBuildTool;

public class TandemBPAssistant : ModuleRules
{
    public TandemBPAssistant(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "Slate", "SlateCore", "UMG"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Projects",               // For FPaths, ProjectContentDir
            "InputCore",              // For key bindings
            "EditorSubsystem",        // For GetEditorSubsystem
            "AssetTools",             // For asset creation
            "EnhancedInput",          // For InputAction/MappingContext
            "KismetCompiler",         // For node manipulation
            "BlueprintGraph"          // For editing Blueprint graphs
        });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "UnrealEd",            // Required for many Editor classes
                "ToolMenus"            // Plugin menus and buttons
            });

            // Optional: disable Unity to avoid shared build issues
            bUseUnity = false;
        }
    }
}
