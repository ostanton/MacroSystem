// Fill out your copyright notice in the Description page of Project Settings.

#include "Macros/DelayMacro.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

UDelayMacro::UDelayMacro()
{
	Name = FText::FromString("Delay");
	Description = FText::FromString("Delays execution of the macro for a specified number of seconds");
	DefaultParameters.Add({
		FText::FromString("Duration (seconds)"),
		EMacroParamType::Float,
		"1.0"
	});
}

void UDelayMacro::ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters)
{
	Super::ExecuteCustomParameters_Implementation(Parameters);

	if (!Parameters.IsValidIndex(0))
		return;

	const float Duration {Parameters[0].As<float>()};
	UMacroDebugStatics::PrintSimple("Delay for: " + FString::SanitizeFloat(Duration) + " second(s)");
	
	GetWorld()->GetTimerManager().SetTimer(
		DelayHandle,
		FTimerDelegate::CreateLambda([this]
		{
			UMacroDebugStatics::PrintSimple("Delay finished");
			FinishExecute();
		}),
		Duration,
		false
	);
}
