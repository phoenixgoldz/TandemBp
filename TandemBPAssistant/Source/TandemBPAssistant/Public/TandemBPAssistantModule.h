// TandemBPAssistantModule.h

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Main module interface for the TandemBP Assistant plugin.
 * Handles startup and shutdown logic for registering windows, tools, etc.
 */
class FTandemBPAssistantModule : public IModuleInterface
{
public:
    /** Called when the module is loaded into memory */
    virtual void StartupModule() override;

    /** Called when the module is unloaded from memory */
    virtual void ShutdownModule() override;
};
