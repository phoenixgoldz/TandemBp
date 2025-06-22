#include "STandemBPChatUI.h"
#include "BlueprintAutoBuilder.h"
#include "TandemGameScaffolder.h"
#include "TandemBPCommandProcessor.h"
#include "Editor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EdGraphSchema_K2.h"

void STandemBPChatUI::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot().AutoHeight()
                [
                    SAssignNew(InputBox, SEditableTextBox)
                        .HintText(FText::FromString("Ask Tandem..."))
                        .OnTextCommitted(this, &STandemBPChatUI::OnTextCommitted)
                        .OnKeyDownHandler(this, &STandemBPChatUI::HandleKeyDown)
                ]
                + SVerticalBox::Slot().FillHeight(1).Padding(5)
                [
                    SAssignNew(LogBox, SMultiLineEditableTextBox)
                        .IsReadOnly(true)
                ]
        ];
}

void STandemBPChatUI::OnTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
    if (CommitType == ETextCommit::OnEnter)
    {
        FString Message = Text.ToString();
        PrintLog("🧠 " + Message);
        CommandHistory.Add(Message);
        HistoryIndex = CommandHistory.Num();
        TryProcessCommand(Message);
        InputBox->SetText(FText::GetEmpty());
    }
}

void STandemBPChatUI::PrintLog(const FString& Line)
{
    FString Previous = LogBox->GetText().ToString();
    LogBox->SetText(FText::FromString(Previous + "\n" + Line));
}

FReply STandemBPChatUI::HandleKeyDown(const FGeometry&, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Up)
    {
        if (CommandHistory.IsValidIndex(HistoryIndex - 1))
        {
            HistoryIndex--;
            InputBox->SetText(FText::FromString(CommandHistory[HistoryIndex]));
        }
        return FReply::Handled();
    }
    else if (InKeyEvent.GetKey() == EKeys::Down)
    {
        if (CommandHistory.IsValidIndex(HistoryIndex + 1))
        {
            HistoryIndex++;
            InputBox->SetText(FText::FromString(CommandHistory[HistoryIndex]));
        }
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

void STandemBPChatUI::TryProcessCommand(const FString& Input)
{
    const FString LowerInput = Input.ToLower();

    if (LowerInput.Contains("create blueprint"))
    {
        FString CleanName = "BP_Auto_" + FGuid::NewGuid().ToString(EGuidFormats::Short);
        UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(CleanName, AActor::StaticClass());

        if (BP)
        {
            FBlueprintAutoBuilder::AddVariable(BP, TEXT("Health"), UEdGraphSchema_K2::PC_Float);
            FBlueprintAutoBuilder::AddFunction(BP, TEXT("TakeDamage"));
            FBlueprintAutoBuilder::WireFunction(BP, TEXT("TakeDamage"));
            FBlueprintAutoBuilder::AddEventBeginPlay(BP);

            if (BP->Status == BS_UpToDate)
                PrintLog("✅ Created & compiled: " + CleanName);
            else
                PrintLog("⚠️ Created " + CleanName + " but compile failed. Check Blueprint for errors.");
        }
    }
    else if (LowerInput.StartsWith("create variable "))
    {
        FString VarName = Input.RightChop(16).TrimStartAndEnd();
        if (VarName.IsEmpty())
        {
            PrintLog("⚠️ No variable name provided. Try: create variable Health");
            return;
        }

        UObject* FocusedAsset = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->GetFocusedAsset();
        UBlueprint* TargetBP = Cast<UBlueprint>(FocusedAsset);

        if (!TargetBP)
        {
            PrintLog("❗ No Blueprint is open. Please open one and try again.");
            return;
        }

        FBlueprintAutoBuilder::AddVariable(TargetBP, VarName, UEdGraphSchema_K2::PC_Float);
        PrintLog("✅ Variable '" + VarName + "' added to current Blueprint.");
    }
    else if (LowerInput.StartsWith("start "))
    {
        FString GameType = Input.RightChop(6);
        FTandemGameScaffolder::ScaffoldGameType(GameType);
        PrintLog("🚀 Bootstrapped game type: " + GameType);
    }
    else if (LowerInput == "teach me")
    {
        ShowTeachMeFallback();
    }
    else
    {
        UBlueprint* BP = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->GetFocusedAsset<UBlueprint>();
        if (BP)
        {
            FBlueprintCommandProcessor::ProcessCommand(Input, BP);
            PrintLog("🔧 Attempted to run command on open Blueprint.");
        }
        else
        {
            PrintLog("❓ Command not recognized. Try typing 'teach me' or opening a Blueprint first.");
        }
    }
}

void STandemBPChatUI::ShowTeachMeFallback()
{
    PrintLog("📘 Manual Steps to Create a Variable:\n"
        "1. Open a Blueprint.\n"
        "2. On the left panel, click '+ Variable'.\n"
        "3. Name it, set type (e.g. float), and mark Editable if needed.\n"
        "4. Compile the Blueprint.");
}
