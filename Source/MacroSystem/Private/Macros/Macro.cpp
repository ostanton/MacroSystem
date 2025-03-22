// Fill out your copyright notice in the Description page of Project Settings.

#include "Macros/Macro.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/MacroSubsystem.h"

bool UMacro::HasIcon() const
{
	return Icon.IsValid();
}

void UMacro::ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters)
{
}

void UMacro::ExecuteActions_Implementation()
{
	if (!GetActions())
	{
		UMacroDebugStatics::PrintSimple(
			"UMacro::ExecuteActions_Implementation - GetActions() returned invalid!",
			EMacroPrintSeverity::Warning
		);
		return;
	}

	if (GetActions()->IsEmpty())
	{
		UMacroDebugStatics::PrintSimple(
			"UMacro::ExecuteActions_Implementation - GetActions() is empty, skipping!",
			EMacroPrintSeverity::Message
		);
		// finish executing this macro, it's not an error to have no actions
		FinishExecute();
		return;
	}

	if (!GetActions()->IsValidIndex(NextActionIndex))
	{
		UMacroDebugStatics::PrintSimple(
			"UMacro::ExecuteActions_Implementation - " + FString::FromInt(NextActionIndex) +
				" is an invalid action index!",
			EMacroPrintSeverity::Warning
		);
		return;
	}
	
	// execute the first action, and it continues to the next by itself in UMacro::ActionFinished
	ExecuteAction((*GetActions())[NextActionIndex]);
}

void UMacro::MacroFinished_Implementation()
{
	NextActionIndex = 0;
	ActiveActions.Empty();
	FinishExecute();
}

bool UMacro::IsRunning_Implementation() const
{
	return bRunning && !ActiveActions.IsEmpty();
}

void UMacro::ExecuteAction(FMacroAction& Action)
{
	auto const Macro = NewObject<UMacro>(this, Action.MacroClass);
	Macro->OnMacroFinished.AddDynamic(this, &UMacro::ActionFinished);
	Macro->SetMacroInfo(Action);
	Macro->Execute();
	ActiveActions.Add(Macro);
}

bool UMacro::ExecuteActionByIndex(const int Index)
{
	if (!bAllowsActions)
		return false;
	
	if (auto const Actions = GetActions();
		!Actions || !Actions->IsValidIndex(Index))
		return false;

	ExecuteAction((*GetActions())[Index]);
	return true;
}

bool UMacro::ExecuteNextAction()
{
	return ExecuteActionByIndex(NextActionIndex);
}

void UMacro::FinishExecute(const bool bSuccess)
{
	bRunning = false;
	UMacroDebugStatics::PrintSimple("Finished macro: " + GetMacroName().ToString());
	OnMacroFinished.Broadcast(this, bSuccess);
}

ACharacter* UMacro::GetPlayerCharacter() const
{
	return UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

APlayerController* UMacro::GetPlayerController() const
{
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

TArray<FMacroParameter>* UMacro::GetParameters()
{
	if (!Info)
		return nullptr;

	return &Info->Parameters;
}

const TArray<FMacroParameter>* UMacro::GetParameters() const
{
	if (!Info)
		return nullptr;

	return &Info->Parameters;
}

TArray<FMacroAction>* UMacro::GetActions()
{
	if (!Info)
		return nullptr;

	return &Info->Actions;
}

const TArray<FMacroAction>* UMacro::GetActions() const
{
	if (!Info)
		return nullptr;

	return &Info->Actions;
}

TSharedRef<SWidget> UMacro::CreateMacroWidget(FMacroAction& ActionInfo)
{
	return SNullWidget::NullWidget;
}

void UMacro::ActionFinished(const UMacro* Macro, const bool bSuccess)
{
	ActiveActions.Remove(Macro);
	NextActionIndex++;

	if (!ExecuteNextAction())
	{
		MacroFinished();
	}
}

void UMacro::Execute()
{
	bRunning = true;
	
	if (auto const Parameters = GetParameters())
		ExecuteCustomParameters(*Parameters);
	else
		ExecuteCustomParameters({});

	if (bAllowsActions && bAutoExecuteActions)
		ExecuteActions();
}

void UMacro::SetMacroInfo(FMacroAction& InInfo)
{
	Info = &InInfo;
}

void UMacro::GetMacroInfo(FMacroAction& ReturnValue) const
{
	if (auto const MacroInfo = GetMacroInfo())
		ReturnValue = *MacroInfo;
}

UMacroSubsystem* UMacro::GetMacroSubsystem() const
{
	return GetTypedOuter<UMacroSubsystem>();
}
