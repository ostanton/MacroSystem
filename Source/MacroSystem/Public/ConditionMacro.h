// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MacroAction.h"
#include "ConditionMacro.generated.h"

/**
 * Macro action for conditional macros, contains extra information about it like if it's inverted
 */
USTRUCT(BlueprintType)
struct MACROSYSTEM_API FConditionMacro : public FMacroAction
{
	GENERATED_BODY()

	FConditionMacro() = default;
	FConditionMacro(
		bool bInInverted,
		const TSubclassOf<UMacro>& InMacroClass,
		const TArray<FMacroParameter>& InParameters,
		const TArray<FMacroAction>& InActions = {},
		const FInstancedStruct& InCustomData = {}
	);

	/** Whether the condition is inverted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Condition Macro")
	bool bInverted {false};
};
