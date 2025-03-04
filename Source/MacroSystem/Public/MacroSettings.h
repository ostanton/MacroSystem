// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MacroSettings.generated.h"

class UMacro;
class UMacroGroup;

USTRUCT(BlueprintType)
struct MACROSYSTEM_API FMacroCategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Category")
	FText FriendlyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Category")
	TArray<TSoftClassPtr<UMacro>> MacroClasses;
};

/**
 * Settings for C++-only classes to interact with BP-implemented stuff
 */
UCLASS(Config=Macro, DefaultConfig, DisplayName="Macro Settings")
class MACROSYSTEM_API UMacroSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, DisplayName="Get Macro Settings", Category="Macro Settings")
	static const UMacroSettings* Get();
	
	UFUNCTION(BlueprintPure, DisplayName="Get Macro Categories", Category="Macro Settings")
	static const TMap<FName, FMacroCategory>& GetCategories();

	UFUNCTION(BlueprintPure, Category="Macro Settings|Theme")
	static FColor GetNormalColour();

	UFUNCTION(BlueprintPure, Category="Macro Settings|Theme")
	static FColor GetHoverColour();

	UFUNCTION(BlueprintPure, DisplayName="Are Macro Settings Valid", Category="Macro Settings")
	static bool IsValid();
	
private:
	UPROPERTY(Config, EditAnywhere, Category="Macro Settings")
	TMap<FName, FMacroCategory> Categories;

	UPROPERTY(Config, EditAnywhere, Category="Theme")
	FColor NormalColour {255, 255, 255, 0};

	UPROPERTY(Config, EditAnywhere, Category="Theme")
	FColor HoverColour {255, 255, 255, 25};
};
