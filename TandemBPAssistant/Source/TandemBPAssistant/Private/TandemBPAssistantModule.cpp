#include "TandemBPAssistantModule.h"
#include "STandemBPChatUI.h"
#include "BlueprintAutoBuilder.h"
#include "ToolMenus.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EdGraphSchema_K2.h"

#define LOCTEXT_NAMESPACE "FTandemBPAssistantModule"

static const FName TabName("TandemBPAssistant");

void FTandemBPAssistantModule::StartupModule()
{
    // Ensure tool menus are initialized
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([] {
        UToolMenus::Initialize();
        }));

    // Register main assistant tab
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TabName, FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&) {
        return SNew(SDockTab)
            .TabRole(ETabRole::NomadTab)
            [
                SNew(STandemBPChatUI)
            ];
        }))
        .SetDisplayName(LOCTEXT("TabTitle", "TandemBP Assistant"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);

    // Add menu option in Window > TandemBP Assistant
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([] {
        UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
        FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
        Section.AddMenuEntry(
            "TandemBP Assistant",
            LOCTEXT("OpenTandemTab", "TandemBP Assistant"),
            LOCTEXT("TandemTooltip", "Open the assistant"),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([] {
                FGlobalTabmanager::Get()->TryInvokeTab(TabName);
                }))
        );
        }));

    // Optional: Run automated test on load
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_TestAI"), AActor::StaticClass());
    if (BP)
    {
        FBlueprintAutoBuilder::AddVariable(BP, TEXT("Health"), UEdGraphSchema_K2::PC_Float);
        FBlueprintAutoBuilder::AddFunction(BP, TEXT("PrintHello"));
        FBlueprintAutoBuilder::WireFunction(BP, TEXT("PrintHello"));
    }
}

void FTandemBPAssistantModule::ShutdownModule()
{
    // Safety checks before shutdown
    if (UToolMenus::IsAvailable())
    {
        UToolMenus::UnRegisterStartupCallback(this);
    }

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTandemBPAssistantModule, TandemBPAssistant)
