// Fill out your copyright notice in the Description page of Project Settings.

#include "MacroSettings.h"

const UMacroSettings* UMacroSettings::Get()
{
	return GetDefault<UMacroSettings>();
}

const TMap<FName, FMacroCategory>& UMacroSettings::GetCategories()
{
	return Get()->Categories;
}

bool UMacroSettings::GetDelayActionExecution()
{
	return Get()->bDelayActionExecution;
}

bool UMacroSettings::GetDelayLoopRerun()
{
	return Get()->bDelayLoopRerun;
}

FColor UMacroSettings::GetNormalColour()
{
	return Get()->NormalColour;
}

FColor UMacroSettings::GetHoverColour()
{
	return Get()->HoverColour;
}

bool UMacroSettings::IsValid()
{
	return Get() != nullptr;
}
