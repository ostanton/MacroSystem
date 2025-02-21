// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionLibraries/MacroActionStatics.h"

const TArray<FMacroAction>& UMacroActionStatics::GetActions(const FMacroAction& Action)
{
	return Action.Actions;
}

void UMacroActionStatics::SetActions(FMacroAction& Action, const TArray<FMacroAction>& Actions)
{
	Action.Actions = Actions;
}
