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
	/**
	 * Executes this macro, considering any valid user-set parameters and macro actions
	 */
	UFUNCTION(BlueprintCallable, Category="Macro")
	void Execute();

	/**
	 * Executes this macro (and not its actions) with custom parameters
	 * @param Parameters Parameters for this execution call
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Macro")
	void ExecuteCustomParameters(const TArray<FMacroParameter>& Parameters);

	/**
	 * Executes this macro's FMacroAction info structure's actions, if any, in sequence
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Macro")
	void ExecuteActions();

	/**
	 * Checks if this macro is currently running, that is, is currently executing. It will start running
	 * after @code Execute()@endcode and stop running after @code FinishExecute()@endcode
	 * @return True if this macro is currently running, false otherwise
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category="Macro")
	bool IsRunning() const;

	/**
	 * Sets this macro's action info to a new action. This does not perform a copy.
	 * The original value will probably be changed at some point as this macro is used
	 * @param InInfo Reference to new macro action info
	 */
	UFUNCTION(BlueprintCallable, Category="Macro")
	void SetMacroInfo(UPARAM(Ref) FMacroAction& InInfo);

	/**
	 * Gets a copy of the pointed to macro action info for this macro.
	 * Changes to this copy do not affect the original action info
	 * @param ReturnValue Copy of the macro action info
	 */
	UFUNCTION(BlueprintPure, Category="Macro")
	void GetMacroInfo(FMacroAction& ReturnValue) const;

	UFUNCTION(BlueprintPure, Category="Macro")
	UMacroSubsystem* GetMacroSubsystem() const;

	/* Getters for C++ */

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
	[[nodiscard]] virtual TSharedRef<SWidget> CreateMacroWidget(FMacroAction& ActionInfo);

	/** Called when this macro has finished its execution, passing in whether it was successful or not */
	UPROPERTY(BlueprintAssignable, Category="Macro")
	FOnMacroFinished OnMacroFinished;

protected:
	/* Default virtual implementations of their BlueprintNativeEvent counterparts */
	
	virtual void ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters);
	virtual void ExecuteActions_Implementation();
	virtual void MacroFinished_Implementation();
	virtual bool IsRunning_Implementation() const;

	/** Called when this macro has finished execution. Run cleanup logic here */
	UFUNCTION(BlueprintNativeEvent, Category="Macro")
	void MacroFinished();

	/**
	 * Executes a macro action regardless of if it's an action in this macro's action struct or not.
	 * It becomes an action of this macro object when this is called
	 * @param Action The action to execute within this macro
	 */
	UFUNCTION(BlueprintCallable, Category="Macro")
	void ExecuteAction(FMacroAction& Action);

	/**
	 * Executes an action that exists in this macro's FMacroAction data struct
	 * @param Index Action index
	 * @return True if the action was found and executed, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category="Macro")
	bool ExecuteActionByIndex(int Index);

	/**
	 * Executes the action after the current action that is executed
	 * @return True if we have a next action and it was executed, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category="Macro")
	bool ExecuteNextAction();

	/**
	 * Marks this macro as finished, and notifies any concerned third party objects that we have finished
	 * @param bSuccess Whether this macro was successful in its execution (used in conditional macros)
	 */
	UFUNCTION(BlueprintCallable, Category="Macro")
	void FinishExecute(bool bSuccess = true);

	/*
	 * Helper world functions, as Blueprints cannot substitute the
	 * world context object easily for non-Actor related objects
	 */

	/**
	 * Gets the player character via @code UGameplayStatics::GetPlayerCharacter@endcode 
	 * @return Player Character
	 */
	UFUNCTION(BlueprintPure, Category="Macro")
	ACharacter* GetPlayerCharacter() const;

	/**
	 * Gets the player controller via @code UGameplayStatics::GetPlayerController@endcode 
	 * @return Player Controller
	 */
	UFUNCTION(BlueprintPure, Category="Macro")
	APlayerController* GetPlayerController() const;

	/** The macro's friendly name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	FText Name;

	/** The macro's friendly description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	FText Description;

	/** Soft pointer to a texture to use as the icon for this macro in the macro editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Macro", meta=(AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> Icon;

	/** Defines the parameters this macro takes as inputs, their types, and their default values */
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
	bool bRunning {false};
};
