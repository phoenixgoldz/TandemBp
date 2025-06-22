#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

class STandemBPChatUI : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STandemBPChatUI) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TSharedPtr<SEditableTextBox> InputBox;
    TSharedPtr<SMultiLineEditableTextBox> LogBox;
    TArray<FString> CommandHistory;
    int32 HistoryIndex = -1;

    void OnTextCommitted(const FText& Text, ETextCommit::Type CommitType);
    void PrintLog(const FString& Line);
    void TryProcessCommand(const FString& Input);
    void ShowTeachMeFallback();
    FReply HandleKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
};