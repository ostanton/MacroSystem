// Fill out your copyright notice in the Description page of Project Settings.

#include "UserMacro.h"

FUserMacro::FUserMacro(
	const FText& InName,
	const TSubclassOf<UMacro>& InMacroClass,
	const TArray<FMacroParameter>& InParameters,
	const TArray<FMacroAction>& InActions,
	const FInstancedStruct& CustomData
)
	: FMacroAction(InMacroClass, InParameters, InActions, CustomData)
	, Name(InName)
{}
