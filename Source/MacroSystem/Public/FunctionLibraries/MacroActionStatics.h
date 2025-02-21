// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MacroAction.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MacroActionStatics.generated.h"

/**
 * Blueprint helper functions for macro actions
 */
UCLASS()
class MACROSYSTEM_API UMacroActionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Macro Action Statics")
	static const TArray<FMacroAction>& GetActions(const FMacroAction& Action);

	UFUNCTION(BlueprintCallable, Category="Macro Action Statics")
	static void SetActions(UPARAM(Ref) FMacroAction& Action, const TArray<FMacroAction>& Actions);
};
