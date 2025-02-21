// Fill out your copyright notice in the Description page of Project Settings.

#include "UserMacroHandle.h"
#include "Subsystems/MacroSubsystem.h"

FUserMacroHandle::FUserMacroHandle(const int InIndex, UMacroSubsystem* InMacroSubsystem)
	: Index(InIndex), MacroSubsystem(InMacroSubsystem)
{
}

bool FUserMacroHandle::IsRunning() const
{
	if (!MacroSubsystem)
		return false;

	return MacroSubsystem->IsUserMacroRunning(Index);
}

UMacroSubsystem* FUserMacroHandle::GetMacroSubsystem() const
{
	return MacroSubsystem;
}

const FUserMacro* FUserMacroHandle::GetMacroInfo() const
{
	if (!MacroSubsystem)
		return nullptr;

	return MacroSubsystem->GetUserMacro(Index);
}

const UMacro* FUserMacroHandle::GetMacro() const
{
	if (!MacroSubsystem)
		return nullptr;

	return MacroSubsystem->GetActiveUserMacro(Index);
}

int FUserMacroHandle::GetIndex() const
{
	return Index;
}

bool FUserMacroHandle::IsValid() const
{
	return MacroSubsystem != nullptr
		&& MacroSubsystem->GetUserMacros().IsValidIndex(Index);
}

void FUserMacroHandle::Execute() const
{
	if (!MacroSubsystem)
		return;
	
	MacroSubsystem->ExecuteUserMacro(Index);
}

void FUserMacroHandle::RemoveAndInvalidate()
{
	if (!MacroSubsystem)
		return;

	MacroSubsystem->RemoveUserMacro(Index);
	Index = -1;
	MacroSubsystem = nullptr;
}

void FUserMacroHandle::Rename(const FText& Name) const
{
	if (!MacroSubsystem)
		return;

	MacroSubsystem->SetUserMacroName(Index, Name);
}
