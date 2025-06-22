#pragma once

#include "Widgets/SCompoundWidget.h"

class STandemBPChatOverlay : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STandemBPChatOverlay) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TSharedPtr<SEditableTextBox> InputBox;
    TSharedPtr<SMultiLineEditableTextBox> OutputLog;

    void OnTextCommitted(const FText& Text, ETextCommit::Type CommitType);
    void PrintMessageToChat(const FString& Message);
};