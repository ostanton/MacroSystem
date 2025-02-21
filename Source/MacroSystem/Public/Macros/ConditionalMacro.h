// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Macro.h"
#include "ConditionalMacro.generated.h"

struct FConditionMacro;

/**
 * A macro container which only executes its actions if the condition macro finishes successfully
 */
UCLASS(NotBlueprintable)
class MACROSYSTEM_API UConditionalMacro : public UMacro
{
	GENERATED_BODY()

public:
	UConditionalMacro();

protected:
	virtual void ExecuteCustomParameters_Implementation(const TArray<FMacroParameter>& Parameters) override;
	virtual bool IsRunning_Implementation() const override;
	virtual TSharedPtr<SWidget> CreateMacroWidget(FMacroAction& ActionInfo) override;

	void ExecuteConditionAction();

	[[nodiscard]] FConditionMacro* GetConditionAction();
	[[nodiscard]] const FConditionMacro* GetConditionAction() const;

	UFUNCTION()
	virtual void ConditionActionFinished(const UMacro* Macro, bool bSuccess);

	UPROPERTY()
	TObjectPtr<UMacro> ActiveCondition;
};
