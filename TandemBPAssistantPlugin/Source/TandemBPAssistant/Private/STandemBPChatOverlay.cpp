#include "STandemBPChatOverlay.h"
#include "TandemBPCommandProcessor.h"
#include "BlueprintAutoBuilder.h"
#include "TandemGameScaffolder.h"
#include "Editor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EdGraphSchema_K2.h"

void STandemBPChatOverlay::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot().AutoHeight()
                [
                    SAssignNew(InputBox, SEditableTextBox)
                        .HintText(NSLOCTEXT("TandemBP", "InputHint", "Ask the assistant..."))
                        .OnTextCommitted(this, &STandemBPChatOverlay::OnTextCommitted)
                ]
                + SVerticalBox::Slot().FillHeight(1.0f).Padding(5)
                [
                    SAssignNew(OutputLog, SMultiLineEditableTextBox)
                        .IsReadOnly(true)
                ]
        ];
}

void STandemBPChatOverlay::OnTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
    if (CommitType != ETextCommit::OnEnter) return;

    const FString Input = Text.ToString();
    PrintMessageToChat("🧠 " + Input);
    InputBox->SetText(FText::GetEmpty());

    FString LowerInput = Input.ToLower();

    if (LowerInput.Contains("create blueprint"))
    {
        UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint("BP_" + FGuid::NewGuid().ToString(EGuidFormats::Short), AActor::StaticClass());
        if (BP)
        {
            FBlueprintAutoBuilder::AddVariable(BP, TEXT("Health"), UEdGraphSchema_K2::PC_Float);
            FBlueprintAutoBuilder::AddFunction(BP, TEXT("TakeDamage"));
            FBlueprintAutoBuilder::WireFunction(BP, TEXT("TakeDamage"));
            FBlueprintAutoBuilder::AddEventBeginPlay(BP);
            PrintMessageToChat("✅ Blueprint created with Health, TakeDamage and BeginPlay.");
        }
        else PrintMessageToChat("❌ Failed to create blueprint.");
    }
    else if (LowerInput.StartsWith("create variable "))
    {
        FString VarName = Input.RightChop(16).TrimStartAndEnd();
        UBlueprint* BP = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->GetFocusedAsset<UBlueprint>();
        if (BP && !VarName.IsEmpty())
        {
            FBlueprintAutoBuilder::AddVariable(BP, VarName, UEdGraphSchema_K2::PC_Float);
            PrintMessageToChat("✅ Variable '" + VarName + "' added.");
        }
        else PrintMessageToChat("⚠️ No blueprint open or invalid name.");
    }
    else if (LowerInput.StartsWith("start "))
    {
        FString GameType = Input.RightChop(6).TrimStartAndEnd();
        FTandemGameScaffolder::ScaffoldGameType(GameType);
        PrintMessageToChat("🚀 Game type '" + GameType + "' scaffolded.");
    }
    else if (LowerInput == "teach me")
    {
        PrintMessageToChat("📘 Examples:\n"
            "- create blueprint\n"
            "- create variable Health\n"
            "- start FPS\n"
            "- start ThirdPerson\n"
            "💡 Try different Unreal templates!");
    }
    else
    {
        UBlueprint* BP = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->GetFocusedAsset<UBlueprint>();
        FBlueprintCommandProcessor::ProcessCommand(Input, BP);
        PrintMessageToChat("🔎 Trying command on open blueprint...");
    }
}

void STandemBPChatOverlay::PrintMessageToChat(const FString& Message)
{
    OutputLog->SetText(FText::FromString(OutputLog->GetText().ToString() + "\n" + Message));
}
