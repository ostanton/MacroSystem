﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Macros/WhileLoopMacro.h"

#include "MacroSettings.h"

UWhileLoopMacro::UWhileLoopMacro()
{
	Name = FText::FromString("While");
	Description = FText::FromString(
		"Continually executes its containing actions as long as the condition action finishes successfully"
	);
	bAllowsActions = true;
	bAutoExecuteActions = false;
}

void UWhileLoopMacro::MacroFinished_Implementation()
{
	if (!UMacroSettings::GetDelayLoopRerun())
	{
		ExecuteConditionAction();
		return;
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]
	{
		ExecuteConditionAction();
	}));
}
