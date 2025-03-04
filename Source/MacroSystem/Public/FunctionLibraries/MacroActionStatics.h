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
	/**
	 * Gets the actions contained in the target macro action
	 * @param Action Macro action
	 * @return Actions within the target macro action
	 */
	UFUNCTION(BlueprintPure, Category="Macro Action Statics")
	static const TArray<FMacroAction>& GetActions(const FMacroAction& Action);

	/**
	 * Sets the contained actions of the target action
	 * @param Action Target action to set
	 * @param Actions Actions to use
	 */
	UFUNCTION(BlueprintCallable, Category="Macro Action Statics")
	static void SetActions(UPARAM(Ref) FMacroAction& Action, const TArray<FMacroAction>& Actions);
};
