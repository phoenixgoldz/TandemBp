#include "BlueprintAutoBuilder.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_CallFunction.h"
#include "K2Node_Event.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphSchema_K2.h"
#include "Editor.h"
#include "Subsystems/AssetEditorSubsystem.h"

UBlueprint* FBlueprintAutoBuilder::CreateBlueprint(const FString& Name, UClass* ParentClass)
{
    const FString Path = FString("/Game/TandemAuto/") + Name;
    UPackage* Package = CreatePackage(*Path);
    UBlueprint* NewBP = FKismetEditorUtilities::CreateBlueprint(
        ParentClass, Package, *Name, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), FName("TandemBP")
    );
    FKismetEditorUtilities::CompileBlueprint(NewBP);
    return NewBP;
}

void FBlueprintAutoBuilder::AddVariable(UBlueprint* Blueprint, const FString& VarName, const FName& TypeCategory)
{
    FEdGraphPinType PinType;
    PinType.PinCategory = TypeCategory;
    FBlueprintEditorUtils::AddMemberVariable(Blueprint, *VarName, PinType);
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
}

void FBlueprintAutoBuilder::AddFunction(UBlueprint* Blueprint, const FString& FuncName)
{
    UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(Blueprint, *FuncName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
    FBlueprintEditorUtils::AddFunctionGraph(Blueprint, Graph, false, nullptr);
    if (!Graph) return;

    const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

    UK2Node_FunctionEntry* EntryNode = nullptr;
    for (UEdGraphNode* Node : Graph->Nodes)
    {
        EntryNode = Cast<UK2Node_FunctionEntry>(Node);
        if (EntryNode) break;
    }

    if (!EntryNode)
    {
        EntryNode = NewObject<UK2Node_FunctionEntry>(Graph);
        Graph->AddNode(EntryNode);
    }

    UK2Node_CallFunction* PrintNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_CallFunction>(
        Graph, FVector2D(200, 100), EK2NewNodeFlags::None,
        [Func = UKismetSystemLibrary::StaticClass()->FindFunctionByName("PrintString")]() {
            auto* Node = NewObject<UK2Node_CallFunction>();
            Node->FunctionReference.SetExternalMember(Func->GetFName(), UKismetSystemLibrary::StaticClass());
            return Node;
        }()
    );

    if (PrintNode)
    {
        PrintNode->GetNodeComment() = FString::Printf(TEXT("Hello from TandemBP Function: %s"), *FuncName);
        UEdGraphPin* ExecIn = EntryNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);
        UEdGraphPin* PrintExecIn = PrintNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
        Schema->TryCreateConnection(ExecIn, PrintExecIn);
    }

    Schema->AddComment(Graph, FVector2D(0, 0), TEXT("Auto-wired function graph with PrintString node"));
    FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
}

void FBlueprintAutoBuilder::WireFunction(UBlueprint* Blueprint, const FString& FuncName)
{
    for (UEdGraph* Graph : Blueprint->FunctionGraphs)
    {
        if (Graph->GetName() == FuncName)
        {
            for (UEdGraphNode* Node : Graph->Nodes)
            {
                if (UK2Node_FunctionEntry* Entry = Cast<UK2Node_FunctionEntry>(Node))
                {
                    Entry->NodeComment = TEXT("Auto-generated function");
                    break;
                }
            }
        }
    }
}

void FBlueprintAutoBuilder::AddEventBeginPlay(UBlueprint* Blueprint)
{
    UEdGraph* Graph = FBlueprintEditorUtils::FindEventGraph(Blueprint);
    if (!Graph) return;

    const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

    UK2Node_Event* BeginPlayNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_Event>(
        Graph, FVector2D(100, 100), EK2NewNodeFlags::None, nullptr);

    BeginPlayNode->EventSignatureName = "ReceiveBeginPlay";
    BeginPlayNode->bOverrideFunction = true;
    BeginPlayNode->CustomFunctionName = "ReceiveBeginPlay";

    UK2Node_CallFunction* PrintNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_CallFunction>(
        Graph, FVector2D(400, 100), EK2NewNodeFlags::None,
        [Func = UKismetSystemLibrary::StaticClass()->FindFunctionByName("PrintString")]() {
            auto* Node = NewObject<UK2Node_CallFunction>();
            Node->FunctionReference.SetExternalMember(Func->GetFName(), UKismetSystemLibrary::StaticClass());
            return Node;
        }()
    );

    if (PrintNode)
    {
        PrintNode->GetNodeComment() = TEXT("Hello from BeginPlay");
        UEdGraphPin* ExecIn = BeginPlayNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);
        UEdGraphPin* PrintExecIn = PrintNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
        Schema->TryCreateConnection(ExecIn, PrintExecIn);
    }

    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
}

UBlueprint* FBlueprintAutoBuilder::GetOpenBlueprint()
{
    if (GEditor)
    {
        UObject* FocusedAsset = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->GetFocusedAsset();
        return Cast<UBlueprint>(FocusedAsset);
    }
    return nullptr;
}
