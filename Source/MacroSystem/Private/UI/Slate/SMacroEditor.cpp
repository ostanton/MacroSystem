#include "SMacroEditor.h"
#include "SActionContainer.h"
#include "Subsystems/MacroSubsystem.h"
#include "SUserMacroButton.h"
#include "SRenameMacro.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Input/SButton.h"

void SMacroEditor::Construct(const FArguments& InArgs)
{
	MacroSubsystem = InArgs._MacroSubsystem;

	ChildSlot
	[
		SNew(SSplitter)
		.Orientation(Orient_Horizontal)
		+SSplitter::Slot()
		.MinSize(256.f)
		.SizeRule(SSplitter::FractionOfParent)
		.Value(0.3f)
		[
			SNew(SBorder)
			.Content()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.Padding(4.f)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(4.f)
					[
						SNew(STextBlock)
						.Text(FText::FromString("User Macros"))
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(NewMacroButton, SButton)
						.Text(FText::FromString("+"))
						.ToolTipText(FText::FromString("Create a new macro"))
						.OnClicked(this, &SMacroEditor::NewMacroClicked)
					]
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(UserMacrosListBox, SListView<FUserMacroItem>)
					.ListItemsSource(&UserMacroArray)
					.SelectionMode(ESelectionMode::Single)
					.OnGenerateRow(this, &SMacroEditor::GenerateUserMacroItem)
					.OnSelectionChanged(this, &SMacroEditor::UserMacroSelected)
				]
			]
		]
		+SSplitter::Slot()
		.MinSize(512.f)
		.SizeRule(SSplitter::FractionOfParent)
		[
			SAssignNew(ActiveMacroSwitcher, SWidgetSwitcher)
			+SWidgetSwitcher::Slot()
			[
				SNew(SBorder)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Create a macro to start"))
				]
			]
			+SWidgetSwitcher::Slot()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBorder)
					.Padding(4.f)
					.Content()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.Padding(4.f)
						[
							SAssignNew(ActiveMacroTextBlock, STextBlock)
							.Text(FText::FromString("No macro selected"))
						]
					]
				]
				+SVerticalBox::Slot()
				.Padding(0.f, 4.f, 0.f, 0.f)
				[
					SNew(SBorder)
					.Padding(4.f)
					.Content()
					[
						SAssignNew(ActionContainer, SActionContainer)
						.ActionsSource(GetSelectedMacroActions())
					]
				]
			]
		]
	];

	if (MacroSubsystem)
		SelectedUserMacro = MacroSubsystem->GetUserMacros().IsEmpty() ? -1 : 0;
	else
		SelectedUserMacro = -1;
	
	RefreshCreatedMacros(SelectedUserMacro);
	RefreshActiveMacroArea();
}

void SMacroEditor::SetMacroSubsystem(UMacroSubsystem* Subsystem)
{
	MacroSubsystem = Subsystem;
}

void SMacroEditor::RefreshCreatedMacros(const int SelectedItem)
{
	if (!MacroSubsystem)
		return;
	
	ActiveMacroSwitcher->SetActiveWidgetIndex(!MacroSubsystem->GetUserMacros().IsEmpty());
	
	UserMacroArray.Empty();
	for (int Index {0}; Index < MacroSubsystem->GetUserMacros().Num(); Index++)
	{
		UserMacroArray.Add(MakeShared<FUserMacroHandle>(Index, MacroSubsystem));
	}

	UserMacrosListBox->RebuildList();
	
	// set the selected item to either be the current selected one, or a target one
	// (given they're both valid indices of course)
	if (const int SelectedItemIndex {SelectedItem == -1 ? SelectedUserMacro : SelectedItem};
		UserMacroArray.IsValidIndex(SelectedItemIndex))
	{
		UserMacrosListBox->SetSelection(UserMacroArray[SelectedItemIndex]);
	}
}

void SMacroEditor::SelectUserMacro(const FUserMacroHandle& Handle)
{
	// ignore if this is already selected
	if (SelectedUserMacro == Handle.GetIndex())
		return;

	SelectedUserMacro = Handle.GetIndex();
	RefreshActiveMacroArea();
}

void SMacroEditor::UserMacroFinished()
{
	if (MacroSubsystem)
		MacroSubsystem->NativeOnUserMacroFinished.Remove(UserMacroFinishedDelegate);
	
	SetEnabled(true);
}

FReply SMacroEditor::NewMacroClicked()
{
	SRenameMacro::PushMenu(
		SharedThis(this),
		NewMacroButton->GetPaintSpaceGeometry().GetAbsolutePosition() +
			FSlateApplication::Get().FindWidgetWindow(SharedThis(this))->GetPositionInScreen() +
			FVector2D {0, NewMacroButton->GetPaintSpaceGeometry().GetAbsoluteSize().Y},
		FText::FromString("Macro name"),
		FOnRenameMacroCommitted::CreateRaw(this, &SMacroEditor::NewMacroNameCommitted)
	);
	
	return FReply::Handled();
}

void SMacroEditor::NewMacroNameCommitted(const FText& Text)
{
	if (!MacroSubsystem)
		return;
	
	SelectedUserMacro = MacroSubsystem->AddUserMacro(Text, {}).GetIndex();
	RefreshCreatedMacros();
	RefreshActiveMacroArea();
}

void SMacroEditor::RefreshActiveMacroArea()
{
	if (!MacroSubsystem)
		return;
	
	if (auto const Macro = MacroSubsystem->GetUserMacro(SelectedUserMacro))
	{
		ActiveMacroTextBlock->SetText(Macro->Name);
		ActionContainer->Refresh(GetSelectedMacroActions());
		return;
	}
	
	ActiveMacroTextBlock->SetText(FText::FromString("Invalid macro selected"));
	ActionContainer->Refresh();
}

TSharedRef<ITableRow> SMacroEditor::GenerateUserMacroItem(
	FUserMacroItem UserMacro,
	const TSharedRef<STableViewBase>& OwnerTable
)
{
	return SNew(STableRow<FUserMacroItem>, OwnerTable)
		[
			SNew(SUserMacroButton)
			.UserMacroHandle(UserMacro)
			.OnRenamed_Lambda([this](const TSharedRef<FUserMacroHandle>& Handle)
			{
				RefreshCreatedMacros(Handle->GetIndex());
				RefreshActiveMacroArea();
			})
			.OnDeleted(this, &SMacroEditor::UserMacroDeleted)
			.OnExecuted(this, &SMacroEditor::UserMacroExecuted)
		];
}

void SMacroEditor::UserMacroSelected(const FUserMacroItem Handle, const ESelectInfo::Type)
{
	if (Handle)
		SelectUserMacro(*Handle);
}

void SMacroEditor::UserMacroDeleted(const int Index)
{
	if (!MacroSubsystem)
		return;
	
	// if the just deleted macro was the selected one
	if (SelectedUserMacro == Index)
	{
		// select the first macro if it exists, or none if not
		if (MacroSubsystem->GetUserMacro(0))
			SelectedUserMacro = 0;
		else
			SelectedUserMacro = -1;
	}
	
	RefreshCreatedMacros(SelectedUserMacro);
	RefreshActiveMacroArea();
}

void SMacroEditor::UserMacroExecuted()
{
	if (MacroSubsystem)
		UserMacroFinishedDelegate =
			MacroSubsystem->NativeOnUserMacroFinished.AddRaw(this, &SMacroEditor::UserMacroFinished);
	
	SetEnabled(false);
}

TArray<FMacroAction>* SMacroEditor::GetSelectedMacroActions() const
{
	if (!MacroSubsystem)
		return nullptr;
	
	if (MacroSubsystem->GetUserMacros().IsEmpty())
		return nullptr;

	if (auto const Macro = MacroSubsystem->GetUserMacro(SelectedUserMacro))
		return &Macro->Actions;

	return nullptr;
}
