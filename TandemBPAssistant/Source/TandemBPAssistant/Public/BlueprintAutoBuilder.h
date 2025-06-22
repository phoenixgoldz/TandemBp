#pragma once
#include "CoreMinimal.h"
#include "Kismet2/BlueprintEditorUtils.h"

class FBlueprintAutoBuilder
{
public:
    static UBlueprint* CreateBlueprint(const FString& Name, UClass* ParentClass);
    static void AddVariable(UBlueprint* Blueprint, const FString& VarName, const FName& TypeCategory);
    static void AddFunction(UBlueprint* Blueprint, const FString& FuncName);
    static void WireFunction(UBlueprint* Blueprint, const FString& FuncName);
    static void AddEventBeginPlay(UBlueprint* Blueprint);
    static UBlueprint* GetOpenBlueprint();
};
