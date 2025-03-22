// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MacroEditor.generated.h"

class UMacroSubsystem;
class SMacroEditor;

/**
 * Wrapper for the Slate macro editor, so it can be used in BP.
 * It is not extendable or usable in the UMG editor, however
 */
UCLASS(NotBlueprintable)
class MACROSYSTEM_API UMacroEditor final : public UWidget
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void OnWidgetRebuilt() override;

	TSharedPtr<SMacroEditor> MyMacroEditor;
};
