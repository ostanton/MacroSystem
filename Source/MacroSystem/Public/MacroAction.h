// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InstancedStruct.h"
#include "MacroParameter.h"
#include "Templates/SubclassOf.h"
#include "MacroAction.generated.h"

class UMacro;

/**
 * Holds data for a macro before it's created, as that data cannot be stored in the macro itself because
 * the macro does not exist in memory yet
 */
USTRUCT(BlueprintType)
struct FMacroAction
{
	GENERATED_BODY()

	FMacroAction() = default;
	FMacroAction(
		const TSubclassOf<UMacro>& InMacroClass,
		const TArray<FMacroParameter>& InParameters,
		const TArray<FMacroAction>& InActions = {},
		const FInstancedStruct& InCustomData = {}
	);
	FMacroAction(const FMacroAction&);
	FMacroAction(FMacroAction&&) noexcept = default;
	FMacroAction& operator=(const FMacroAction&);
	FMacroAction& operator=(FMacroAction&&) noexcept = default;

	[[nodiscard]] UMacro* GetDefaultMacro() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Action")
	TSubclassOf<UMacro> MacroClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Action")
	TArray<FMacroParameter> Parameters;

	TArray<FMacroAction> Actions;

	/**
	 * Custom struct data for specialised macros (e.g. conditional's condition action,
	 * for loop's max iteration, etc.). Meant to be opaque to anything except for the
	 * macro used in MacroClass, and the associated widget.
	 *
	 * It is initialised in the widget which is related to its related macro specialisation.
	 * For example, a for loop has MaxIterations, which is represented visually with
	 * @code SLoopIterationMacro@endcode. That widget initialises the custom data with an
	 * @code FLoopIteration@endcode struct, which then has its members filled with whatever
	 * widgets are contained in the macro widget. That data is then got with the @code UForLoopMacro@endcode
	 * macro. This is all seamless, as there are no copies created. Any pointer to data is to the data owned
	 * by whatever macro this structure is an action of
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Macro Action")
	FInstancedStruct CustomData;
};
