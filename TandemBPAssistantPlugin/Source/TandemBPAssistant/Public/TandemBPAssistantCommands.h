#pragma once

#include "Framework/Commands/Commands.h"
#include "Styling/SlateStyle.h"

class FTandemBPAssistantCommands : public TCommands<FTandemBPAssistantCommands>
{
public:
    FTandemBPAssistantCommands();
    virtual void RegisterCommands() override;

    TSharedPtr<FUICommandInfo> OpenPluginWindow;
    TSharedPtr<FUICommandInfo> ToggleChatOverlay;
    TSharedPtr<FUICommandInfo> ScaffoldGame;
    TSharedPtr<FUICommandInfo> ClearLog;
};