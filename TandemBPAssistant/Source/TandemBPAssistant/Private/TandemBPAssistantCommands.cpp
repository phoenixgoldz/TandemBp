#include "TandemBPAssistantCommands.h"
#include "TandemBPAssistantStyle.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

FTandemBPAssistantCommands::FTandemBPAssistantCommands()
    : TCommands<FTandemBPAssistantCommands>(
        TEXT("TandemBPAssistant"),
        NSLOCTEXT("Contexts", "TandemBPAssistant", "TandemBP Assistant"),
        NAME_None,
        FTandemBPAssistantStyle::GetStyleSetName())
{
}

void FTandemBPAssistantCommands::RegisterCommands()
{
    UI_COMMAND(OpenPluginWindow, "TandemBP Assistant", "Open the plugin window", EUserInterfaceActionType::Button, FInputChord());
    UI_COMMAND(ToggleChatOverlay, "Toggle Chat Overlay", "Show/hide floating assistant chat window", EUserInterfaceActionType::ToggleButton, FInputChord());
    UI_COMMAND(ScaffoldGame, "Scaffold Game", "Generate project structure and Blueprints for common templates", EUserInterfaceActionType::Button, FInputChord());
    UI_COMMAND(ClearLog, "Clear Log", "Clear the assistant log output window", EUserInterfaceActionType::Button, FInputChord());
}
