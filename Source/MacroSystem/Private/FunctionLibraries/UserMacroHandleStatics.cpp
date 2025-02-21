// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionLibraries/UserMacroHandleStatics.h"

bool UUserMacroHandleStatics::IsRunning(const FUserMacroHandle& Handle)
{
	return Handle.IsRunning();
}

const UMacro* UUserMacroHandleStatics::GetUserMacro(const FUserMacroHandle& Handle)
{
	return Handle.GetMacro();
}

void UUserMacroHandleStatics::Execute(const FUserMacroHandle& Handle)
{
	Handle.Execute();
}

int UUserMacroHandleStatics::GetIndex(const FUserMacroHandle& Handle)
{
	return Handle.GetIndex();
}

bool UUserMacroHandleStatics::IsValid(const FUserMacroHandle& Handle)
{
	return Handle.IsValid();
}

void UUserMacroHandleStatics::RemoveAndInvalidate(FUserMacroHandle& Handle)
{
	Handle.RemoveAndInvalidate();
}
