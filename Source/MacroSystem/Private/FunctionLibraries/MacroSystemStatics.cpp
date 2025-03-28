﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionLibraries/MacroSystemStatics.h"
#include "Subsystems/MacroSubsystem.h"
#include "UI/MacroEditor.h"
#include "Engine/World.h"

UMacroEditor* UMacroSystemStatics::CreateMacroEditorWidget(UObject* Outer)
{
	if (!Outer->GetWorld())
		return nullptr;

	auto const MacroEditor = NewObject<UMacroEditor>(Outer);

	return MacroEditor;
}

void UMacroSystemStatics::CreateMacroEditorWindow(
	const UObject* WorldContext,
	const FText Title,
	const FVector2D Size,
	const bool bResizable
)
{
	auto const World = WorldContext->GetWorld();
	if (!World)
		return;

	auto const MacroSubsystem = World->GetGameInstance()->GetSubsystem<UMacroSubsystem>();
	if (!MacroSubsystem)
		return;

	MacroSubsystem->CreateMacroEditorWindow(Title, Size, bResizable);
}
