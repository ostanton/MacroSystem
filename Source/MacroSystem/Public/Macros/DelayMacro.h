// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Macro.h"
#include "Engine/TimerHandle.h"
#include "DelayMacro.generated.h"

/**
 * Basic macro for waiting a set amount of time before finishing. It serves as an example of how to create a custom
 * C++-based macro in this system.
 */
UCLASS()
class MACROSYSTEM_API UDelayMacro : public UMacro
{
	GENERATED_BODY()

public:
	UDelayMacro();

protected:
	/* Begin UMacro */
	virtual void ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters) override;
	/* End UMacro */

private:
	FTimerHandle DelayHandle;
};
