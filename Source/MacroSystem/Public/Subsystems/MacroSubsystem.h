// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UserMacro.h"
#include "UserMacroHandle.h"
#include "Save/MacroSaveData.h"
#include "Templates/SharedPointer.h"
#include "MacroSubsystem.generated.h"

class SWindow;

DECLARE_MULTICAST_DELEGATE(FNativeOnUserMacroFinished);

/**
 * Subsystem to manage created macros and run them. Indices index into the array of FMacroAction
 * unless specified otherwise
 */
UCLASS()
class MACROSYSTEM_API UMacroSubsystem final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Creates a user macro and adds it to the subsystem
	 * @param Name User macro name
	 * @param Actions Actions contained in the user macro
	 * @return Handle to created user macro
	 */
	UFUNCTION(BlueprintCallable, Category="Macro Subsystem")
	FUserMacroHandle AddUserMacro(FText Name, const TArray<FMacroAction>& Actions);

	UFUNCTION(BlueprintCallable, Category="Macro Subsystem")
	bool AddActionToUserMacro(int Index, const FMacroAction& Action);

	UFUNCTION(BlueprintCallable, Category="Macro Subsystem")
	bool SetUserMacroActions(int Index, const TArray<FMacroAction>& Actions);

	UFUNCTION(BlueprintCallable, Category="Macro Subsystem")
	bool RemoveUserMacro(int Index);

	UFUNCTION(BlueprintCallable, Category="Macro Subsystem")
	void ExecuteUserMacro(int Index);

	UFUNCTION(BlueprintCallable, Category="Macro Subsystem")
	void SetUserMacroName(int Index, const FText& Name);

	UFUNCTION(BlueprintPure, Category="Macro Subsystem")
	int GetLastUserMacroIndex() const;

	[[nodiscard]] FUserMacro* GetUserMacro(int Index);
	[[nodiscard]] const FUserMacro* GetUserMacro(int Index) const;
	[[nodiscard]] const FUserMacro* GetUserMacroByName(const FText& Name) const;

	/**
	 * Gets the active user macro via an index into the user macros array
	 * @param Index Index into the user macros array
	 * @return Active user macro or nullptr if not active
	 */
	[[nodiscard]] const UMacro* GetActiveUserMacro(int Index) const;

	UFUNCTION(BlueprintPure, Category="Macro Subsystem")
	bool DoesUserMacroWithNameExist(const FText& Name) const;

	UFUNCTION(BlueprintPure, Category="Macro Subsystem")
	bool IsUserMacroRunning(int Index) const;

	UFUNCTION(BlueprintPure, Category="Macro Subsystem")
	bool IsAnyUserMacroRunning() const;

	[[nodiscard]] const TArray<FUserMacro>& GetUserMacros() const;

	/**
	 * Gets all the user macros the subsystem manages via their handles
	 * @return Handles to all existing user macros
	 */
	UFUNCTION(BlueprintPure, DisplayName="Get User Macros", Category="Macro Subsystem")
	TArray<FUserMacroHandle> GetUserMacros_BP();

	UFUNCTION(BlueprintCallable, Category="Macro Subsystem")
	void LoadFromSaveData(const FMacroSaveData& SaveData);

	UFUNCTION(BlueprintPure, Category="Macro Subsystem")
	void SaveToSaveData(FMacroSaveData& SaveData) const;

	/**
	 * Creates the macro editor window via Slate
	 */
	void CreateMacroEditorWindow(
		const FText& Title = INVTEXT("User Macro Editor"),
		const FVector2D& Size = {1280, 720},
		bool bResizable = true
	);

	FNativeOnUserMacroFinished NativeOnUserMacroFinished;

private:
	UFUNCTION()
	void UserMacroFinished(const UMacro* Macro, bool bSuccess);
	
	TArray<FUserMacro> UserMacros;
	UPROPERTY()
	TArray<TObjectPtr<const UMacro>> ActiveUserMacros;

	TWeakPtr<SWindow> MacroEditorWindow;
};
