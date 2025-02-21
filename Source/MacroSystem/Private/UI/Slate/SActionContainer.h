#pragma once

#include "Widgets/SCompoundWidget.h"

class UMacroSubsystem;
class SMacroAction;
struct FMacroAction;
class SMacroActionContextButton;

/**
 * Widget that holds a list of created actions for a given UCompoundMacro
 */
class SActionContainer final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SActionContainer)
		: _ActionsSource(nullptr)
	{}
		SLATE_ARGUMENT(TArray<FMacroAction>*, ActionsSource)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void Refresh(TArray<FMacroAction>* NewSource = nullptr);

	/**
	 * Actually creates the macro action data and adds it to the selected user macro's actions array in
	 * the macro subsystem. Moves the input action into the target array
	 * @param Action Action to create
	 */
	void CreateAction(FMacroAction Action);

private:
	void ShowContextMenu(const FVector2D& Position);
	void CreateActionWidget(FMacroAction& Action);
	void ActionButtonClicked(const SMacroActionContextButton* Button);
	void ActionDeleted(const TSharedRef<SMacroAction>& Action);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	TSharedPtr<SVerticalBox> ActionVerticalBox;
	TSharedPtr<SButton> AddActionButton;

	TArray<FMacroAction>* ActionsSource {nullptr};
};
