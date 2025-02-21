#pragma once

#include "Widgets/SCompoundWidget.h"

class SEditableTextBox;

DECLARE_DELEGATE_OneParam(FOnRenameMacroCommitted, const FText&);

/**
 * Wrapper for the rename text box popup, so it can be easily created anywhere
 */
class SRenameMacro final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRenameMacro)
	{}
		SLATE_ARGUMENT(FText, InitialText)
		/** Called when the text is committed successfully (i.e. the text is not empty, etc.) */
		SLATE_EVENT(FOnRenameMacroCommitted, OnTextCommitted)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	static TSharedRef<SRenameMacro> PushMenu(
		const TSharedRef<SWidget>& Parent,
		const FVector2D& Position,
		const FText& Text,
		const FOnRenameMacroCommitted& OnTextCommitted
	);

	[[nodiscard]] TSharedRef<SEditableTextBox> GetRenameTextBox() const;

private:
	[[nodiscard]] bool CanCommitText(const FText& Text) const;

	FOnRenameMacroCommitted OnRenameMacroCommitted;
	
	TWeakPtr<IMenu> OwningMenu;
	TSharedPtr<SEditableTextBox> RenameTextBox;
};
