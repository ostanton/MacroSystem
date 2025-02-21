#include "SRenameMacro.h"
#include "Widgets/Input/SEditableTextBox.h"

void SRenameMacro::Construct(const FArguments& InArgs)
{
	OnRenameMacroCommitted = InArgs._OnTextCommitted;
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Name"))
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(RenameTextBox, SEditableTextBox)
			.Text(InArgs._InitialText)
			.HintText(FText::FromString("Macro name must be valid"))
			.SelectAllTextWhenFocused(true)
			.OnTextCommitted_Lambda([this](const FText& Text, const ETextCommit::Type Type)
			{
				// only do on enter, as it is also called when losing focus
				// (it loses focus when its owning menu is dismissed, calling this again)
				if (Type != ETextCommit::OnEnter)
					return;
				
				if (CanCommitText(Text))
				{
					[[maybe_unused]] auto Temp = OnRenameMacroCommitted.ExecuteIfBound(Text);
					
					if (const auto Menu = OwningMenu.Pin())
						Menu->Dismiss();
				}
			})
		]
	];
}

TSharedRef<SRenameMacro> SRenameMacro::PushMenu(
	const TSharedRef<SWidget>& Parent,
	const FVector2D& Position,
	const FText& Text,
	const FOnRenameMacroCommitted& OnTextCommitted
)
{
	TSharedRef<SRenameMacro> Result = SNew(SRenameMacro)
		.InitialText(Text)
		.OnTextCommitted(OnTextCommitted);

	Result->OwningMenu = FSlateApplication::Get().PushMenu(
		Parent,
		{},
		Result,
		Position,
		FPopupTransitionEffect::ContextMenu
	);
	
	FSlateApplication::Get().SetKeyboardFocus(Result->GetRenameTextBox());

	return Result;
}

TSharedRef<SEditableTextBox> SRenameMacro::GetRenameTextBox() const
{
	return RenameTextBox.ToSharedRef();
}

bool SRenameMacro::CanCommitText(const FText& Text) const
{
	return !Text.IsEmpty();
}
