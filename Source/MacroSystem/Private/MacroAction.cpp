// Fill out your copyright notice in the Description page of Project Settings.

#include "MacroAction.h"

#include "FunctionLibraries/MacroDebugStatics.h"
#include "Macros/Macro.h"

FMacroAction::FMacroAction(
	const TSubclassOf<UMacro>& InMacroClass,
	const TArray<FMacroParameter>& InParameters,
	const TArray<FMacroAction>& InActions,
	const FInstancedStruct& InCustomData
)
	: MacroClass(InMacroClass)
	, Parameters(InParameters)
	, Actions(InActions)
	, CustomData(InCustomData)
{}

FMacroAction::FMacroAction(const FMacroAction&)
{
	// UClass requires copy constructor and assignment for reflection! So just print an error instead of deleting
	UMacroDebugStatics::PrintSimple(
		"Illegal FMacroAction copy!",
		EMacroPrintSeverity::Error,
		10.f
	);
}

FMacroAction& FMacroAction::operator=(const FMacroAction&)
{
	UMacroDebugStatics::PrintSimple(
		"Illegal FMacroAction copy!",
		EMacroPrintSeverity::Error,
		10.f
	);
	return *this;
}

UMacro* FMacroAction::GetDefaultMacro() const
{
	return Cast<UMacro>(MacroClass->ClassDefaultObject);
}
