#include "TandemBPAssistantStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr<FSlateStyleSet> FTandemBPAssistantStyle::StyleInstance = nullptr;

void FTandemBPAssistantStyle::Initialize()
{
    if (!StyleInstance.IsValid())
    {
        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FTandemBPAssistantStyle::Shutdown()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

FName FTandemBPAssistantStyle::GetStyleSetName()
{
    static FName StyleSetName(TEXT("TandemBPAssistantStyle"));
    return StyleSetName;
}

TSharedRef<FSlateStyleSet> FTandemBPAssistantStyle::Create()
{
    TSharedRef<FSlateStyleSet> StyleSet = MakeShareable(new FSlateStyleSet("TandemBPAssistantStyle"));

    const FVector2D Icon40x40(40.0f, 40.0f);
    StyleSet->Set("TandemBPAssistant.OpenPluginWindow",
        new FSlateImageBrush(IPluginManager::Get().FindPlugin("TandemBPAssistant")->GetBaseDir() / TEXT("Resources/Icon128.png"), Icon40x40));

    return StyleSet;
}

const ISlateStyle& FTandemBPAssistantStyle::Get()
{
    return *StyleInstance;
}
