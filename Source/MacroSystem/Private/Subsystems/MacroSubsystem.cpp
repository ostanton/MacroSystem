// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/MacroSubsystem.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "Macros/Macro.h"
#include "UI/Slate/SMacroEditor.h"
#include "Framework/Application/SlateApplication.h"

FUserMacroHandle UMacroSubsystem::AddUserMacro(const FText Name, const TArray<FMacroAction>& Actions)
{
	return {
		UserMacros.Emplace(Name, UMacro::StaticClass(), TArray<FMacroParameter> {}, Actions),
		this
	};
}

bool UMacroSubsystem::AddActionToUserMacro(const int Index, const FMacroAction& Action)
{
	if (!UserMacros.IsValidIndex(Index))
		return false;

	UserMacros[Index].Actions.Add(Action);
	return true;
}

bool UMacroSubsystem::SetUserMacroActions(const int Index, const TArray<FMacroAction>& Actions)
{
	if (!UserMacros.IsValidIndex(Index))
		return false;

	UserMacros[Index].Actions = Actions;
	return true;
}

bool UMacroSubsystem::RemoveUserMacro(const int Index)
{
	if (!UserMacros.IsValidIndex(Index))
		return false;

	UserMacros.RemoveAt(Index);
	return true;
}

void UMacroSubsystem::ClearUserMacros()
{
	UserMacros.Empty();
}

void UMacroSubsystem::ExecuteUserMacro(const int Index)
{
	if (!UserMacros.IsValidIndex(Index))
		return;

	auto& MacroInfo = UserMacros[Index];
	UMacroDebugStatics::PrintSimple("Executing user macro: " + MacroInfo.Name.ToString());
	auto const Macro = NewObject<UMacro>(this, MacroInfo.MacroClass);
	Macro->OnMacroFinished.AddUniqueDynamic(this, &UMacroSubsystem::UserMacroFinished);
	Macro->bAllowsActions = true;
	Macro->bAutoExecuteActions = true;
	Macro->SetMacroInfo(MacroInfo);
	Macro->Execute();
	ActiveUserMacros.Add(Macro);
}

void UMacroSubsystem::SetUserMacroName(const int Index, const FText& Name)
{
	if (!UserMacros.IsValidIndex(Index))
		return;

	UserMacros[Index].Name = Name;
}

int UMacroSubsystem::GetLastUserMacroIndex() const
{
	return UserMacros.Num() - 1;
}

FUserMacro* UMacroSubsystem::GetUserMacro(const int Index)
{
	if (!UserMacros.IsValidIndex(Index))
		return nullptr;

	return &UserMacros[Index];
}

const FUserMacro* UMacroSubsystem::GetUserMacro(const int Index) const
{
	if (!UserMacros.IsValidIndex(Index))
		return nullptr;

	return &UserMacros[Index];
}

const FUserMacro* UMacroSubsystem::GetUserMacroByName(const FText& Name) const
{
	for (auto& Macro : UserMacros)
	{
		if (Macro.Name.EqualTo(Name))
			return &Macro;
	}

	return nullptr;
}

const UMacro* UMacroSubsystem::GetActiveUserMacro(const int Index) const
{
	if (!IsUserMacroRunning(Index))
		return nullptr;

	for (const auto Macro : ActiveUserMacros)
	{
		if (Macro->GetMacroInfo() == GetUserMacro(Index))
			return Macro;
	}
	
	return nullptr;
}

bool UMacroSubsystem::DoesUserMacroWithNameExist(const FText& Name) const
{
	return GetUserMacroByName(Name) != nullptr;
}

bool UMacroSubsystem::IsUserMacroRunning(const int Index) const
{
	if (!UserMacros.IsValidIndex(Index))
		return false;

	for (const auto Macro : ActiveUserMacros)
	{
		if (Macro->GetMacroInfo() == &UserMacros[Index])
			return true;
	}

	return false;
}

bool UMacroSubsystem::IsAnyUserMacroRunning() const
{
	return !ActiveUserMacros.IsEmpty();
}

void UMacroSubsystem::StopAllActiveMacros()
{
	for (auto Macro : ActiveUserMacros)
	{
		NativeOnUserMacroFinished.Broadcast();
	}

	ActiveUserMacros.Empty();
}

const TArray<FUserMacro>& UMacroSubsystem::GetUserMacros() const
{
	return UserMacros;
}

TArray<FUserMacroHandle> UMacroSubsystem::GetUserMacros_BP()
{
	TArray<FUserMacroHandle> Result;
	for (int i {0}; i < UserMacros.Num(); i++)
	{
		Result.Emplace(i, this);
	}
	return Result;
}

void UMacroSubsystem::LoadFromSaveData(const FMacroSaveData& SaveData)
{
	UserMacros = SaveData.UserMacros;
}

void UMacroSubsystem::SaveToSaveData(FMacroSaveData& SaveData) const
{
	SaveData.UserMacros = UserMacros;
}

void UMacroSubsystem::CreateMacroEditorWindow(
	const FText& Title,
	const FVector2D& Size,
	const bool bResizable
)
{
	// don't create a window if one already exists
	if (MacroEditorWindow.IsValid())
		return;
	
	auto const Window = SNew(SWindow)
		.Title(Title)
		.ClientSize(Size)
		.SizingRule(bResizable ? ESizingRule::UserSized : ESizingRule::FixedSize)
		.Content()
		[
			SNew(SMacroEditor)
			.MacroSubsystem(this)
		];

	MacroEditorWindow = Window;
	FSlateApplication::Get().AddWindowAsNativeChild(
		Window,
		FSlateApplication::Get().GetActiveTopLevelWindow().ToSharedRef()
	);
}

void UMacroSubsystem::UserMacroFinished(const UMacro* Macro, bool bSuccess)
{
	NativeOnUserMacroFinished.Broadcast();
	UMacroDebugStatics::PrintSimple(
		"Finished executing user macro: " + static_cast<const FUserMacro*>(Macro->GetMacroInfo())->Name.ToString()
	);
	ActiveUserMacros.Remove(Macro);
}
