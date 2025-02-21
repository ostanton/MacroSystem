// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionLibraries/MacroDebugStatics.h"
#include "Engine/Engine.h"

void UMacroDebugStatics::PrintSimple(const FString& String, const EMacroPrintSeverity Severity, const float Duration)
{
	if (!GEngine)
		return;
	
	FColor Colour {FColor::Green};
	switch (Severity)
	{
	case EMacroPrintSeverity::Message:
		Colour = FColor::Green;
		break;
	case EMacroPrintSeverity::Warning:
		Colour = FColor::Yellow;
		break;
	case EMacroPrintSeverity::Error:
		Colour = FColor::Red;
		break;
	}
	GEngine->AddOnScreenDebugMessage(-1, Duration, Colour, "Macro System: " + String);
}
