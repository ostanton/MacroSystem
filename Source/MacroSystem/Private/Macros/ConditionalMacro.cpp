// Fill out your copyright notice in the Description page of Project Settings.

#include "Macros/ConditionalMacro.h"
#include "ConditionMacro.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "UI/Slate/Macros/SConditionMacro.h"

UConditionalMacro::UConditionalMacro()
{
	Name = FText::FromString("If");
	Description = FText::FromString("Executes its containing actions if the condition action finish successfully");
	bAllowsActions = true;
	bAutoExecuteActions = false;
}

void UConditionalMacro::ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters)
{
	Super::ExecuteCustomParameters_Implementation(Parameters);

	// don't execute the condition if it doesn't exist!
	if (!GetConditionAction() || !GetConditionAction()->MacroClass)
	{
		FinishExecute();
		return;
	}
	
	ExecuteConditionAction();
}

bool UConditionalMacro::IsRunning_Implementation() const
{
	return Super::IsRunning_Implementation() || ActiveCondition != nullptr;
}

TSharedRef<SWidget> UConditionalMacro::CreateMacroWidget(FMacroAction& ActionInfo)
{
	return SNew(SConditionMacro)
		.Condition(&ActionInfo.CustomData);
}

void UConditionalMacro::ExecuteConditionAction()
{
	ActiveCondition = NewObject<UMacro>(this, GetConditionAction()->MacroClass);
	ActiveCondition->OnMacroFinished.AddUniqueDynamic(this, &UConditionalMacro::ConditionActionFinished);
	ActiveCondition->SetMacroInfo(*GetConditionAction());
	ActiveCondition->Execute();
}

FConditionMacro* UConditionalMacro::GetConditionAction()
{
	if (Info)
		return Info->CustomData.GetMutablePtr<FConditionMacro>();

	return nullptr;
}

const FConditionMacro* UConditionalMacro::GetConditionAction() const
{
	if (Info)
		return Info->CustomData.GetPtr<FConditionMacro>();

	return nullptr;
}

void UConditionalMacro::ConditionActionFinished(const UMacro* Macro, const bool bSuccess)
{
	// narrowing conversion to explicitly cast
	const bool bSucceeded {static_cast<bool>(bSuccess ^ Info->CustomData.Get<FConditionMacro>().bInverted)};
	
	UMacroDebugStatics::PrintSimple(
		"Conditional " + FString(bSucceeded ? "succeeded" : "failed")
	);
	
	if (bSucceeded)
	{
		ActiveCondition = nullptr;
		NextActionIndex = 0;
		ExecuteActions();
		return;
	}

	// finish execute if the condition is false, skip over it
	FinishExecute(false);
}
