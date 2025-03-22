#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"

class SActionContainer;
struct FMacroAction;
struct FUserMacroHandle;
class SMacroAction;
class SEditableTextBox;
class SScrollBox;
class UMacroSubsystem;
class ITableRow;
class STableViewBase;
class SButton;
class STextBlock;
class SWidgetSwitcher;

/**
 * The main macro editor, which facilitates the creation and editing of user macros, composing them of
 * built-in macros that are added via the context menu
 */
class SMacroEditor final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMacroEditor)
		: _MacroSubsystem(nullptr)
	{}
		SLATE_ARGUMENT(UMacroSubsystem*, MacroSubsystem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetMacroSubsystem(UMacroSubsystem* Subsystem);

private:
	void RefreshCreatedMacros(int SelectedItem = -1);
	void SelectUserMacro(const FUserMacroHandle& Handle);
	void UserMacroFinished();
	FDelegateHandle UserMacroFinishedDelegate;

	FReply NewMacroClicked();
	void NewMacroNameCommitted(const FText& Text);

	void RefreshActiveMacroArea();

	using FUserMacroItem = TSharedPtr<FUserMacroHandle>;
	TArray<FUserMacroItem> UserMacroArray;
	TSharedRef<ITableRow> GenerateUserMacroItem(FUserMacroItem UserMacro, const TSharedRef<STableViewBase>& OwnerTable);
	void UserMacroSelected(FUserMacroItem Handle, ESelectInfo::Type);
	void UserMacroDeleted(int Index);
	void UserMacroExecuted();
	
	UMacroSubsystem* MacroSubsystem {nullptr};

	TSharedPtr<SButton> NewMacroButton;
	TSharedPtr<SListView<FUserMacroItem>> UserMacrosListBox;
	TSharedPtr<SWidgetSwitcher> ActiveMacroSwitcher;
	TSharedPtr<STextBlock> ActiveMacroTextBlock;
	TSharedPtr<SActionContainer> ActionContainer;

	TArray<FMacroAction>* GetSelectedMacroActions() const;

	int SelectedUserMacro {-1};
};
