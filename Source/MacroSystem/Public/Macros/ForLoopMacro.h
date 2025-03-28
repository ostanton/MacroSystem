﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Macro.h"
#include "ForLoopMacro.generated.h"

struct FLoopIteration;

/**
 * Loops through its actions a set number of times. Its iterations are 1-indexed, as they are counts, not indices
 */
UCLASS(NotBlueprintable)
class MACROSYSTEM_API UForLoopMacro : public UMacro
{
	GENERATED_BODY()

public:
	UForLoopMacro();

protected:
	/* Begin UMacro */
	virtual void ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters) override;
	virtual void MacroFinished_Implementation() override;
	virtual TSharedRef<SWidget> CreateMacroWidget(FMacroAction& ActionInfo) override;
	/* End UMacro */

	[[nodiscard]] bool CanIterate() const;
	[[nodiscard]] FLoopIteration* GetLoopIterationAction();
	[[nodiscard]] const FLoopIteration* GetLoopIterationAction() const;
	[[nodiscard]] int* GetMaxIterations();
	[[nodiscard]] const int* GetMaxIterations() const;
	
	int CurrentIteration {1};
};
