// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserMacro.h"
#include "UObject/Object.h"
#include "MacroSaveData.generated.h"

/**
 * Struct containing macro-related save data
 */
USTRUCT(BlueprintType)
struct FMacroSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUserMacro> UserMacros;
};
