// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MacroDebugStatics.generated.h"

UENUM()
enum class EMacroPrintSeverity : uint8
{
	Message,
	Warning,
	Error
};

/**
 * Static functions used for debugging. As a Blueprint Function Library so it interops with BP nicely
 */
UCLASS()
class MACROSYSTEM_API UMacroDebugStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Macro Debug Statics")
	static void PrintSimple(
		const FString& String,
		EMacroPrintSeverity Severity = EMacroPrintSeverity::Message,
		float Duration = 5.f
	);
};
