// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MacroParameter.h"
#include "MacroAction.h"
#include "UObject/Object.h"
#include "Templates/SharedPointer.h"
#include "UObject/SoftObjectPtr.h"
#include "Macro.generated.h"

class UMacroSubsystem;
class UMacro;
class UTexture2D;
class SWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMacroFinished, const UMacro*, Macro, bool, bSuccess);

/**
 * Base class for built-in macros. Derive from this to implement a new macro the player can use to compose
 * their own user macros. Macros by default are leaf macros, they are self-contained and contain no other macros
 */
UCLASS(Blueprintable, BlueprintType, meta=(ShowWorldContextPin))
class MACROSYSTEM_API UMacro : public UObject
{
	GENERATED_BODY()

	friend UMacroSubsystem;

public:
	UFUNCTION(BlueprintCallable, Category="Macro")
	void Execute();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Macro")
	void ExecuteCustomParameters(const TArray<FMacroParameter>& Parameters);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Macro")
	void ExecuteActions();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Macro")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category="Macro")
	void SetMacroInfo(FMacroAction& InInfo);

	UFUNCTION(BlueprintPure, Category="Macro")
	UMacroSubsystem* GetMacroSubsystem() const;

	[[nodiscard]] FMacroAction* GetMacroInfo() {return Info;}
	[[nodiscard]] const FMacroAction* GetMacroInfo() const {return Info;}
	[[nodiscard]] const FText& GetMacroName() const {return Name;}
	[[nodiscard]] const FText& GetMacroDescription() const {return Description;}
	[[nodiscard]] bool HasIcon() const;
	[[nodiscard]] UTexture2D* GetIconLoaded() const {return Icon.LoadSynchronous();}
	[[nodiscard]] const TArray<FMacroParameter>& GetDefaultParameters() const {return DefaultParameters;}
	[[nodiscard]] bool GetAllowsActions() const {return bAllowsActions;}

	[[nodiscard]] TArray<FMacroParameter>* GetParameters();
	[[nodiscard]] const TArray<FMacroParameter>* GetParameters() const;
	[[nodiscard]] TArray<FMacroAction>* GetActions();
	[[nodiscard]] const TArray<FMacroAction>* GetActions() const;

	/**
	 * Creates a Slate widget for a custom macro class for it to expose its own data properties to
	 * @param ActionInfo The action information that represents what this macro class will become once allocated
	 * @return Created widget
	 */
	[[nodiscard]] virtual TSharedPtr<SWidget> CreateMacroWidget(FMacroAction& ActionInfo);

	UPROPERTY(BlueprintAssignable, Category="Macro")
	FOnMacroFinished OnMacroFinished;

protected:
	virtual void ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters);
	virtual void ExecuteActions_Implementation();
	virtual void MacroFinished_Implementation();
	virtual bool IsRunning_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, Category="Macro")
	void MacroFinished();

	UFUNCTION(BlueprintCallable, Category="Macro")
	void ExecuteAction(FMacroAction& Action);

	UFUNCTION(BlueprintCallable, Category="Macro")
	bool ExecuteActionByIndex(int Index);

	UFUNCTION(BlueprintCallable, Category="Macro")
	bool ExecuteNextAction();

	UFUNCTION(BlueprintCallable, Category="Macro")
	void FinishExecute(bool bSuccess = true);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	TArray<FMacroParameter> DefaultParameters;

	/**
	 * Whether this macro supports macros to execute within it as actions. Used in the editor widget
	 * to show/hide the add macro list in the macro
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	bool bAllowsActions {false};

	/**
	 * Whether to automatically execute this macro's actions, if it has any. Turn off to perform
	 * any checks or custom macro execution before running this macro's actions
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	bool bAutoExecuteActions {true};

	/** Pointer to the action data this macro was created from */
	FMacroAction* Info {nullptr};
	int NextActionIndex {0};
	UPROPERTY()
	TArray<TObjectPtr<const UMacro>> ActiveActions;

	UFUNCTION()
	void ActionFinished(const UMacro* Macro, bool bSuccess);

private:
	bool bRunning {true};
};
