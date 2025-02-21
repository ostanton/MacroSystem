// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserMacroHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UserMacroHandleStatics.generated.h"

/**
 * Static functions for FUserMacroHandle that Blueprints can call
 */
UCLASS()
class MACROSYSTEM_API UUserMacroHandleStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="User Macro Handle Statics")
	static bool IsRunning(const FUserMacroHandle& Handle);

	UFUNCTION(BlueprintPure, Category="User Macro Handle Statics")
	static const UMacro* GetUserMacro(const FUserMacroHandle& Handle);

	UFUNCTION(BlueprintCallable, Category="User Macro Handle Statics")
	static void Execute(const FUserMacroHandle& Handle);

	UFUNCTION(BlueprintPure, Category="User Macro Handle Statics")
	static int GetIndex(const FUserMacroHandle& Handle);

	UFUNCTION(BlueprintPure, Category="User Macro Handle Statics")
	static bool IsValid(const FUserMacroHandle& Handle);

	UFUNCTION(BlueprintCallable, Category="User Macro Handle Statics")
	static void RemoveAndInvalidate(UPARAM(Ref) FUserMacroHandle& Handle);
};
