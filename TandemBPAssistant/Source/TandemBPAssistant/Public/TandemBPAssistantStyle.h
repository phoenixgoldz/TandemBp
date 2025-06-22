#pragma once

#include "CoreMinimal.h"

class ISlateStyle;

class FTandemBPAssistantStyle
{
public:
    static void Initialize();
    static void Shutdown();
    static const ISlateStyle& Get();
    static FName GetStyleSetName();

private:
    static TSharedRef<class FSlateStyleSet> Create();
    static TSharedPtr<class FSlateStyleSet> StyleInstance;
};
