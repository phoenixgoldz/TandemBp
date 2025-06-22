#pragma once

#include "CoreMinimal.h"

class UBlueprint;

class FTandemGameScaffolder
{
public:
    static void ScaffoldGameType(const FString& GameType);
    static void SetupEnhancedInput();

private:
    static void CreateFPSCore();
    static void CreateThirdPersonCore();
    static void CreateTopDownCore();
    static void CreateVRCore();
    static void CreateBlankCore();
    static void CreateRPGCore();
    static void CreateCardGameCore();
    static void CreateStrategyCore();
    static void CreateHorrorCore();
    static void CreateVehicleCore();
    static void CreatePuzzleCore();
    static void CreateSideScrollerCore();
    static void AddCommonAssets(const FString& Category);
    static UBlueprint* CreateBlueprintAsset(FString Name, UClass* ParentClass, FString Folder = "/Game/TandemAuto/");
};