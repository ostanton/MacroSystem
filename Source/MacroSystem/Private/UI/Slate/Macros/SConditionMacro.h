#pragma once

#include "Widgets/SCompoundWidget.h"

struct FConditionMacro;
class SMacroAction;
class SMacroActionContextButton;
struct FInstancedStruct;
class SButton;
class SOverlay;

class SConditionMacro final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConditionMacro)
		: _Condition(nullptr)
	{}
		SLATE_ARGUMENT(FInstancedStruct*, Condition)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FInstancedStruct* Condition {nullptr};

	TSharedPtr<SButton> SelectorButton;
	TSharedPtr<SOverlay> SelectedActionContainer;

	void ActionContextButtonClicked(const SMacroActionContextButton* Button);
	void ConditionActionDeleted(const TSharedRef<SMacroAction>& Action);

	void RefreshCondition();
};
