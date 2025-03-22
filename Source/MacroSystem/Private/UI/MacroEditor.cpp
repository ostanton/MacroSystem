// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MacroEditor.h"
#include "Slate/SMacroEditor.h"

TSharedRef<SWidget> UMacroEditor::RebuildWidget()
{
	return SAssignNew(MyMacroEditor, SMacroEditor);
}

void UMacroEditor::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMacroEditor.Reset();
}

void UMacroEditor::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (!IsDesignTime())
	{
		MyMacroEditor->SetMacroSubsystem(GetGameInstance()->GetSubsystem<UMacroSubsystem>());
	}
}
