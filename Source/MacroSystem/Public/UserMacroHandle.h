// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserMacroHandle.generated.h"

class UMacro;
struct FUserMacro;
class UMacroSubsystem;

/**
 * Wrapper around an index into UMacroSubsystem's user macro array,
 * and a pointer to that subsystem for self-contained use
 */
USTRUCT(BlueprintType)
struct MACROSYSTEM_API FUserMacroHandle
{
	GENERATED_BODY()

	// default constructor for reflection stuff
	FUserMacroHandle() = default;
	FUserMacroHandle(int InIndex, UMacroSubsystem* InMacroSubsystem);

	[[nodiscard]] bool IsRunning() const;
	[[nodiscard]] UMacroSubsystem* GetMacroSubsystem() const;
	[[nodiscard]] const FUserMacro* GetMacroInfo() const;
	[[nodiscard]] const UMacro* GetMacro() const;
	[[nodiscard]] int GetIndex() const;
	[[nodiscard]] bool IsValid() const;

	void Execute() const;
	void RemoveAndInvalidate();
	void Rename(const FText& Name) const;

private:
	int Index {-1};
	TObjectPtr<UMacroSubsystem> MacroSubsystem;
};
