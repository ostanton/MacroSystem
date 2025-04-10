// Fill out your copyright notice in the Description page of Project Settings.

#include "Macros/ForLoopMacro.h"
#include "LoopIteration.h"
#include "MacroSettings.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "UI/Slate/Macros/SLoopIterationMacro.h"

UForLoopMacro::UForLoopMacro()
{
	Name = FText::FromString("For");
	Description = FText::FromString("Executes its actions a set number of times");
	bAllowsActions = true;
	bAutoExecuteActions = false;
}

void UForLoopMacro::ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters)
{
	Super::ExecuteCustomParameters_Implementation(Parameters);

	if (!CanIterate())
		return;

	ExecuteActions();
}

void UForLoopMacro::MacroFinished_Implementation()
{
	UMacroDebugStatics::PrintSimple(
		"Finished for loop iteration: " + FString::FromInt(CurrentIteration)
	);

	auto FinishLambda = [this]
	{
		CurrentIteration++;
	
		if (!CanIterate())
		{
			UMacroDebugStatics::PrintSimple(
				"Finished for loop"
			);
			CurrentIteration = 0;
			Super::MacroFinished_Implementation();
			return;
		}
	
		NextActionIndex = 0;
		ExecuteActions();
	};
	
	if (!UMacroSettings::GetDelayLoopRerun())
	{
		FinishLambda();
		return;
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda(FinishLambda));
}

TSharedRef<SWidget> UForLoopMacro::CreateMacroWidget(FMacroAction& ActionInfo)
{
	return SNew(SLoopIterationMacro)
		.IterationData(&ActionInfo.CustomData);
}

bool UForLoopMacro::CanIterate() const
{
	if (auto const MaxIterations = GetMaxIterations())
		return CurrentIteration <= *MaxIterations;

	return false;
}

FLoopIteration* UForLoopMacro::GetLoopIterationAction()
{
	if (Info)
		return Info->CustomData.GetMutablePtr<FLoopIteration>();

	return nullptr;
}

const FLoopIteration* UForLoopMacro::GetLoopIterationAction() const
{
	if (Info)
		return Info->CustomData.GetPtr<FLoopIteration>();

	return nullptr;
}

int* UForLoopMacro::GetMaxIterations()
{
	if (auto const Action = GetLoopIterationAction())
		return &Action->MaxIterations;

	return nullptr;
}

const int* UForLoopMacro::GetMaxIterations() const
{
	if (auto const Action = GetLoopIterationAction())
		return &Action->MaxIterations;

	return nullptr;
}
