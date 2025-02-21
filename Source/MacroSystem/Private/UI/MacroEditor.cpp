// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MacroEditor.h"
#include "Slate/SMacroEditor.h"

void UMacroEditor::SetMacroSubsystem(UMacroSubsystem* InMacroSubsystem)
{
	MacroSubsystem = InMacroSubsystem;
}

TSharedRef<SWidget> UMacroEditor::RebuildWidget()
{
	return SAssignNew(MyMacroEditor, SMacroEditor)
		.MacroSubsystem(MacroSubsystem);
}

void UMacroEditor::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMacroEditor.Reset();
}
