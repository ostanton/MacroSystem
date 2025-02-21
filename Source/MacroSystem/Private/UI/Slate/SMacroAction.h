#pragma once

#include "MacroAction.h"
#include "Widgets/SCompoundWidget.h"

class SActionContainer;
class SActionParameter;
class SMacroAction;
class SBorder;
class SVerticalBox;
class SButton;

DECLARE_DELEGATE_OneParam(FOnMacroActionDeleted, const TSharedRef<SMacroAction>&);

/**
 * Action widget which represents a built-in macro, and allows the user to change its parameters
 * 
 * TODO - allowing reordering of actions
 */
class SMacroAction final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMacroAction)
		: _Action(nullptr)
	{}
		SLATE_ARGUMENT(FMacroAction*, Action)
		SLATE_EVENT(FOnMacroActionDeleted, OnDeleted)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	[[nodiscard]] FMacroAction* GetAction() const {return MacroAction;}

	FOnMacroActionDeleted OnMacroActionDeleted;

private:
	FMacroAction* MacroAction {nullptr};

	TSharedPtr<SBorder> RootBorder;
	TSharedPtr<SVerticalBox> ParameterVerticalBox;
	TSharedPtr<SButton> ExtrasButton;
	TSharedPtr<SActionContainer> ActionContainer;

	void RefreshActions();

	void RefreshParameters();
	void Deleted();

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void ShowContextMenu(const FVector2D& Position);

	FSlateBrush BackgroundBrush;
	FSlateBrush IconBrush;
};
