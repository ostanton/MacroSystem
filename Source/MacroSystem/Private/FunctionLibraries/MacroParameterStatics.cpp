// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionLibraries/MacroParameterStatics.h"

bool UMacroParameterStatics::AsBool(const FMacroParameter& Parameter)
{
	return Parameter.As<bool>();
}

int UMacroParameterStatics::AsInt(const FMacroParameter& Parameter)
{
	return Parameter.As<int>();
}

float UMacroParameterStatics::AsFloat(const FMacroParameter& Parameter)
{
	return Parameter.As<float>();
}

FString UMacroParameterStatics::TypeAsString(const FMacroParameter& Parameter)
{
	return Parameter.TypeAsString();
}

FText UMacroParameterStatics::TypeAsText(const FMacroParameter& Parameter)
{
	return Parameter.TypeAsText();
}
