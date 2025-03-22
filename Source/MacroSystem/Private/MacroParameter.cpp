// Fill out your copyright notice in the Description page of Project Settings.

#include "MacroParameter.h"

template<>
bool FMacroParameter::As() const
{
	return FCString::ToBool(*Value);
}

template<>
int FMacroParameter::As() const
{
	return FCString::Atoi(*Value);
}

template<>
float FMacroParameter::As() const
{
	return FCString::Atof(*Value);
}

template<>
const FString& FMacroParameter::As() const
{
	return Value;
}

FString MacroParamTypeToString(const EMacroParamType Type)
{
	switch (Type)
	{
	case EMacroParamType::Boolean:
		return "Boolean";
	case EMacroParamType::Integer:
		return "Integer";
	case EMacroParamType::Float:
		return "Float";
	case EMacroParamType::String:
		return "String";
	}

	return "<unknown>";
}

bool FMacroParameter::operator==(const FMacroParameter& Other) const
{
	return FriendlyName.IdenticalTo(Other.FriendlyName)
		&& Type == Other.Type
		&& Value == Other.Value;
}

bool FMacroParameter::operator!=(const FMacroParameter& Other) const
{
	return !(*this == Other);
}

FText FMacroParameter::TypeAsText() const
{
	return FText::FromString(TypeAsString());
}

FString FMacroParameter::TypeAsString() const
{
	return MacroParamTypeToString(Type);
}
