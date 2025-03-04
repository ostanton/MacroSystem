// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionalMacro.h"
#include "WhileLoopMacro.generated.h"

/**
 * Executes its containing macros while its condition finishes successfully
 */
UCLASS()
class MACROSYSTEM_API UWhileLoopMacro : public UConditionalMacro
{
	GENERATED_BODY()

public:
	UWhileLoopMacro();

protected:
	/* Begin UMacro */
	virtual void MacroFinished_Implementation() override;
	/* End UMacro */
};
