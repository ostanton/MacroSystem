// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MacroParameter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MacroParameterStatics.generated.h"

/**
 * Blueprint helper functions for macro parameters
 */
UCLASS()
class MACROSYSTEM_API UMacroParameterStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Macro Parameter Statics")
	static int AsInt(const FMacroParameter& Parameter);

	UFUNCTION(BlueprintPure, Category="Macro Parameter Statics")
	static float AsFloat(const FMacroParameter& Parameter);

	UFUNCTION(BlueprintPure, Category="Macro Parameter Statics")
	static FString TypeAsString(const FMacroParameter& Parameter);

	UFUNCTION(BlueprintPure, Category="Macro Parameter Statics")
	static FText TypeAsText(const FMacroParameter& Parameter);
};
