// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MacroParameter.generated.h"

/**
 * All the supported types used in macro parameters
 */
UENUM(BlueprintType)
enum class EMacroParamType : uint8
{
	Integer,
	Float,
	String
};

/**
 * A more "user-friendly" string representation of the macro type enum value, as
 * the built-in UEnum::GetValueAsString displays the enum class itself also, which is not desired
 * @param Type Param type
 * @return String version
 */
FString MacroParamTypeToString(EMacroParamType Type);

/**
 * A single parameter for a macro (not including user macros). Its value is stored as a generic string, which is
 * then got via the templated @code As<T>()@endcode function. It only supports the types found in
 * @code EMacroParamType@endcode 
 */
USTRUCT(BlueprintType)
struct MACROSYSTEM_API FMacroParameter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Parameter")
	FText FriendlyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Parameter")
	EMacroParamType Type {EMacroParamType::Integer};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Parameter")
	FString Value;

	template<typename T>
	T As() const
	{
		unimplemented();
		return 0;
	}

	FString TypeAsString() const;
	FText TypeAsText() const;
};

// specialisations for the supported parameter types

template<> int FMacroParameter::As() const;
template<> float FMacroParameter::As() const;
template<> const FString& FMacroParameter::As() const;
