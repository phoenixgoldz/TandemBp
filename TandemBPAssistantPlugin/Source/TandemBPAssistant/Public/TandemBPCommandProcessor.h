#pragma once

#include "CoreMinimal.h"

class UBlueprint;

class FBlueprintCommandProcessor
{
public:
    static void ProcessCommand(const FString& Command, UBlueprint* TargetBlueprint);

private:
    static void CreateVariable(UBlueprint* Blueprint, const FString& VarName, const FString& TypeName);
    static void CreateFunction(UBlueprint* Blueprint, const FString& FunctionName);
};
