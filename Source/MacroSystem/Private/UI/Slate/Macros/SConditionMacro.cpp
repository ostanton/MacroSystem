#include "SConditionMacro.h"
#include "ConditionMacro.h"
#include "InstancedStruct.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "Macros/Macro.h"
#include "UI/Slate/MacroActionContextMenu.h"
#include "UI/Slate/SMacroAction.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"

void SConditionMacro::Construct(const FArguments& InArgs)
{
	Condition = InArgs._Condition;
	checkf(Condition, TEXT("SConditionMacro::Construct - Condition is invalid!"));

	// the specific fields will be set when selecting a condition macro
	if (!Condition->IsValid())
	{
		Condition->InitializeAs<FConditionMacro>(
			false,
			nullptr,
			TArray<FMacroParameter> {}
		);
	}

	ChildSlot
	[
		SNew(SBox)
		.MinDesiredWidth(256.f)
		.MaxDesiredWidth(512.f)
		.Content()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Condition:"))
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SAssignNew(SelectorButton, SButton)
					.Text(FText::FromString("Edit"))
					.ToolTipText(FText::FromString("Selects a macro to use for this condition"))
					.OnClicked_Lambda([this]
					{
						FMacroActionContextMenu::Show(
							SharedThis(this),
							SelectorButton->GetPaintSpaceGeometry().GetAbsolutePosition() +
								FSlateApplication::Get().FindWidgetWindow(SharedThis(this))->GetPositionInScreen() +
								FVector2D {0, SelectorButton->GetPaintSpaceGeometry().GetAbsoluteSize().Y},
							FOnActionContextButtonClicked::CreateRaw(this, &SConditionMacro::ActionContextButtonClicked)
						);
						
						return FReply::Handled();
					})
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(InvertedCheckbox, SCheckBox)
					.ToolTipText(FText::FromString("Inverts this condition when checked"))
					.OnCheckStateChanged(this, &SConditionMacro::InvertCheckboxStateChanged)
				]
				+SHorizontalBox::Slot()
				.FillWidth(1.f)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(SelectedActionContainer, SOverlay)
				]
			]
		]
	];

	RefreshCondition();
}

void SConditionMacro::ActionContextButtonClicked(const SMacroActionContextButton* Button)
{
	auto& Macro = Condition->GetMutable<FConditionMacro>();
	Macro.MacroClass = Button->GetMacro()->GetClass();
	Macro.Parameters = Button->GetMacro()->GetDefaultParameters();

	RefreshCondition();
}

void SConditionMacro::ConditionActionDeleted(const TSharedRef<SMacroAction>& Action)
{
	auto& Macro = Condition->GetMutable<FConditionMacro>();
	Macro.MacroClass = nullptr;
	Macro.Parameters.Empty();

	RefreshCondition();
}

void SConditionMacro::InvertCheckboxStateChanged(const ECheckBoxState State)
{
	switch (State)
	{
	case ECheckBoxState::Unchecked:
		Condition->GetMutable<FConditionMacro>().bInverted = false;
		break;
	case ECheckBoxState::Checked:
		Condition->GetMutable<FConditionMacro>().bInverted = true;
		break;
	default:
		break;
	}

	UMacroDebugStatics::PrintSimple(
		"Condition changed to " + Condition->Get<FConditionMacro>().bInverted
	);
}

void SConditionMacro::RefreshCondition()
{
	SelectedActionContainer->ClearChildren();

	// if we have a valid condition macro
	if (Condition->IsValid())
	{
		if (Condition->Get<FConditionMacro>().MacroClass)
		{
			SelectedActionContainer->AddSlot()
			[
				SNew(SMacroAction)
				.Action(Condition->GetMutablePtr<FConditionMacro>())
				.OnDeleted(this, &SConditionMacro::ConditionActionDeleted)
			];
		}

		InvertedCheckbox->SetIsChecked(
			Condition->Get<FConditionMacro>().bInverted ? ECheckBoxState::Checked : ECheckBoxState::Unchecked
		);
	}
}
