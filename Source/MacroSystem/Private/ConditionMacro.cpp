// Fill out your copyright notice in the Description page of Project Settings.

#include "ConditionMacro.h"

FConditionMacro::FConditionMacro(
	const bool bInInverted,
	const TSubclassOf<UMacro>& InMacroClass,
	const TArray<FMacroParameter>& InParameters,
	const TArray<FMacroAction>& InActions,
	const FInstancedStruct& InCustomData
)
	: FMacroAction(InMacroClass, InParameters, InActions, InCustomData)
	, bInverted(bInInverted)
{}
