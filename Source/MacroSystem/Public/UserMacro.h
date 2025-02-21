// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MacroAction.h"
#include "UserMacro.generated.h"

/**
 * Extended FMacroAction specifically for user-created macros
 */
USTRUCT(BlueprintType)
struct FUserMacro : public FMacroAction
{
	GENERATED_BODY()

	FUserMacro() = default;
	FUserMacro(
		const FText& InName,
		const TSubclassOf<UMacro>& InMacroClass,
		const TArray<FMacroParameter>& InParameters,
		const TArray<FMacroAction>& InActions,
		const FInstancedStruct& CustomData = {}
	);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="User Macro")
	FText Name;
};
