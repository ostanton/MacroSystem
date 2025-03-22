// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MacroSystemStatics.generated.h"

class UMacroEditor;

/**
 * General functions for macro use
 */
UCLASS()
class MACROSYSTEM_API UMacroSystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Macro System Statics", meta=(WorldContext="WorldContext"))
	static UMacroEditor* CreateMacroEditorWidget(UObject* Outer);

	UFUNCTION(BlueprintCallable, Category="Macro System Statics", meta=(WorldContext="WorldContext"))
	static void CreateMacroEditorWindow(
		const UObject* WorldContext,
		FText Title = INVTEXT("User Macro Editor"),
		FVector2D Size = FVector2D(1280, 720),
		bool bResizable = true
	);
};
