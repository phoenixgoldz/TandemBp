#include "TandemGameScaffolder.h"
#include "BlueprintAutoBuilder.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "K2Node_CallFunction.h"
#include "K2Node_DynamicCast.h"
#include "K2Node_Event.h"
#include "EdGraphSchema_K2.h"
#include "EdGraph/EdGraph.h"

void FTandemGameScaffolder::ScaffoldGameType(const FString& GameType)
{
    const FString Lower = GameType.ToLower();

    if (Lower.Contains("first") || Lower.Contains("fps")) CreateFPSCore();
    else if (Lower.Contains("third")) CreateThirdPersonCore();
    else if (Lower.Contains("top")) CreateTopDownCore();
    else if (Lower.Contains("vr")) CreateVRCore();
    else if (Lower.Contains("blank")) CreateBlankCore();
    else if (Lower.Contains("rpg")) CreateRPGCore();
    else if (Lower.Contains("card")) CreateCardGameCore();
    else if (Lower.Contains("strategy")) CreateStrategyCore();
    else if (Lower.Contains("horror")) CreateHorrorCore();
    else if (Lower.Contains("vehicle")) CreateVehicleCore();
    else if (Lower.Contains("puzzle")) CreatePuzzleCore();
    else if (Lower.Contains("side") || Lower.Contains("sidescroller")) CreateSideScrollerCore();
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown GameType: %s"), *GameType);
        return;
    }

    // ✅ Always scaffold shared components
    SetupEnhancedInput();
}

void FTandemGameScaffolder::SetupEnhancedInput()
{
    // Generate Enhanced Input Assets
    FBlueprintAutoBuilder::CreateBlueprint("IMC_Default", UInputMappingContext::StaticClass());
    FBlueprintAutoBuilder::CreateBlueprint("IA_Move", UInputAction::StaticClass());
    FBlueprintAutoBuilder::CreateBlueprint("IA_Jump", UInputAction::StaticClass());
    FBlueprintAutoBuilder::CreateBlueprint("IA_Look", UInputAction::StaticClass());

    // HUD Widget Scaffold
    UBlueprint* HUDWidget = FBlueprintAutoBuilder::CreateBlueprint("BP_HUDWidget", UUserWidget::StaticClass());
    FBlueprintAutoBuilder::AddVariable(HUDWidget, "PlayerHealth", UEdGraphSchema_K2::PC_Float);
    FBlueprintAutoBuilder::AddFunction(HUDWidget, "UpdateHealthBar");
}

// -------------------------- CORE BUILDERS --------------------------

void FTandemGameScaffolder::CreateFPSCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_FPSCharacter"), APawn::StaticClass());
    FBlueprintAutoBuilder::AddVariable(BP, TEXT("Ammo"), UEdGraphSchema_K2::PC_Int);
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("FireWeapon"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateThirdPersonCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_ThirdPersonCharacter"), ACharacter::StaticClass());
    FBlueprintAutoBuilder::AddFunction(BP, "Jump");
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);

    // 🔌 Optional: Wire Enhanced Input logic inside BeginPlay
    UEdGraph* Graph = FBlueprintEditorUtils::FindEventGraph(BP);
    if (!Graph) return;

    const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

    UK2Node_CallFunction* GetControllerNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_CallFunction>(
        Graph, FVector2D(100, 300), EK2NewNodeFlags::None,
        [Func = AActor::StaticClass()->FindFunctionByName("GetController")]()
        {
            auto* Node = NewObject<UK2Node_CallFunction>();
            Node->FunctionReference.SetExternalMember(Func->GetFName(), AActor::StaticClass());
            return Node;
        }()
            );

    UK2Node_DynamicCast* CastNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_DynamicCast>(
        Graph, FVector2D(400, 300), EK2NewNodeFlags::None);
    CastNode->SetTargetType(APlayerController::StaticClass());

    UK2Node_Event* BeginPlayNode = nullptr;
    for (UEdGraphNode* Node : Graph->Nodes)
    {
        if (auto* EventNode = Cast<UK2Node_Event>(Node))
        {
            if (EventNode->EventSignatureName == "ReceiveBeginPlay")
            {
                BeginPlayNode = EventNode;
                break;
            }
        }
    }

    if (BeginPlayNode && GetControllerNode && CastNode)
    {
        Schema->TryCreateConnection(
            BeginPlayNode->FindPinChecked(UEdGraphSchema_K2::PN_Then),
            GetControllerNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute));

        Schema->TryCreateConnection(
            GetControllerNode->GetReturnValuePin(),
            CastNode->GetCastSourcePin());

        Schema->TryCreateConnection(
            GetControllerNode->FindPinChecked(UEdGraphSchema_K2::PN_Then),
            CastNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute));
    }
}

void FTandemGameScaffolder::CreateTopDownCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_TopDownController"), APlayerController::StaticClass());
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("MoveToClickLocation"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateVRCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_VRHandController"), AActor::StaticClass());
    FBlueprintAutoBuilder::AddVariable(BP, TEXT("GripStrength"), UEdGraphSchema_K2::PC_Float);
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateBlankCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_BlankActor"), AActor::StaticClass());
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateRPGCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_RPGCharacter"), ACharacter::StaticClass());
    FBlueprintAutoBuilder::AddVariable(BP, TEXT("Level"), UEdGraphSchema_K2::PC_Int);
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("GainXP"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateCardGameCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_DeckManager"), AActor::StaticClass());
    FBlueprintAutoBuilder::AddVariable(BP, TEXT("Deck"), UEdGraphSchema_K2::PC_Object);
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("DrawCard"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateStrategyCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_UnitManager"), AActor::StaticClass());
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("SelectUnit"));
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("IssueCommand"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateHorrorCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_GhostAI"), AAIController::StaticClass());
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("HauntPlayer"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateVehicleCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_Vehicle"), APawn::StaticClass());
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("Drive"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreatePuzzleCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_PuzzleManager"), AActor::StaticClass());
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("CheckSolution"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}

void FTandemGameScaffolder::CreateSideScrollerCore()
{
    UBlueprint* BP = FBlueprintAutoBuilder::CreateBlueprint(TEXT("BP_SideScrollerCharacter"), ACharacter::StaticClass());
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("Jump"));
    FBlueprintAutoBuilder::AddFunction(BP, TEXT("MoveSideways"));
    FBlueprintAutoBuilder::AddEventBeginPlay(BP);
}
