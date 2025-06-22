#include "TandemBPCommandProcessor.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "EdGraph/EdGraph.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_VariableSet.h"

void FBlueprintCommandProcessor::ProcessCommand(const FString& Command, UBlueprint* TargetBlueprint)
{
    if (!TargetBlueprint) return;

    FString Cmd = Command.ToLower();
    if (Cmd.StartsWith("create variable"))
    {
        FString VarName;
        Command.Split(TEXT("create variable"), nullptr, &VarName);
        VarName = VarName.TrimStartAndEnd();
        CreateVariable(TargetBlueprint, VarName, TEXT("float"));
    }
    else if (Cmd.StartsWith("add function"))
    {
        FString FuncName;
        Command.Split(TEXT("add function"), nullptr, &FuncName);
        FuncName = FuncName.TrimStartAndEnd();
        CreateFunction(TargetBlueprint, FuncName);
    }
}

void FBlueprintCommandProcessor::CreateVariable(UBlueprint* Blueprint, const FString& VarName, const FString& TypeName)
{
    FEdGraphPinType PinType;
    PinType.PinCategory = UEdGraphSchema_K2::PC_Float;
    FBlueprintEditorUtils::AddMemberVariable(Blueprint, *VarName, PinType);
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
}

void FBlueprintCommandProcessor::CreateFunction(UBlueprint* Blueprint, const FString& FunctionName)
{
    UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(Blueprint, *FunctionName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
    FBlueprintEditorUtils::AddFunctionGraph(Blueprint, NewGraph, false, nullptr);
}
