// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LoopIteration.generated.h"

/**
 * Macro action for loops with a limit on iterations, among other potential things
 */
USTRUCT(BlueprintType)
struct FLoopIteration
{
	GENERATED_BODY()

	FLoopIteration() = default;
	FLoopIteration(int InMaxIterations);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loop Iteration")
	int MaxIterations {2};
};
