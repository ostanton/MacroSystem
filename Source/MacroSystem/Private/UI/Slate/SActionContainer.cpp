#include "SActionContainer.h"
#include "MacroActionContextMenu.h"
#include "SMacroAction.h"
#include "SMacroActionContextButton.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "Macros/Macro.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"

void SActionContainer::Construct(const FArguments& InArgs)
{
	ActionsSource = InArgs._ActionsSource;
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Actions"))
		]
		+SVerticalBox::Slot()
		[
			SNew(SScrollBox)
			+SScrollBox::Slot()
			[
				SAssignNew(ActionVerticalBox, SVerticalBox)
			]
			+SScrollBox::Slot()
			.HAlign(HAlign_Right)
			.Padding(0.f, 4.f, 0.f, 0.f)
			[
				SAssignNew(AddActionButton, SButton)
				.Text(FText::FromString("Add"))
				.OnClicked_Lambda([this]
				{
					ShowContextMenu(
						AddActionButton->GetPaintSpaceGeometry().GetAbsolutePosition() +
						FSlateApplication::Get().FindWidgetWindow(SharedThis(this))->GetPositionInScreen() +
						FVector2D {0.f, AddActionButton->GetPaintSpaceGeometry().GetAbsoluteSize().Y}
					);
					return FReply::Handled();
				})
			]
		]
	];
}

void SActionContainer::Refresh(TArray<FMacroAction>* NewSource)
{
	ActionsSource = NewSource;

	if (!ActionsSource)
	{
		UMacroDebugStatics::PrintSimple(
			"SActionContainer::Refresh - ActionsSource is invalid!",
			EMacroPrintSeverity::Warning
		);
		return;
	}

	ActionVerticalBox->ClearChildren();

	if (ActionsSource->IsEmpty())
	{
		UMacroDebugStatics::PrintSimple(
			"SActionContainer::Refresh - ActionsSource is empty!"
		);
		return;
	}
	
	for (auto& Action : *ActionsSource)
	{
		CreateActionWidget(Action);
	}
}

void SActionContainer::CreateAction(FMacroAction Action)
{
	if (!ActionsSource)
	{
		UMacroDebugStatics::PrintSimple(
			"SActionContainer::CreateAction - ActionsSource is invalid!",
			EMacroPrintSeverity::Warning
		);
		return;
	}

	// move the action in, we never want to copy a macro action
	const auto Index = ActionsSource->Add(MoveTemp(Action));
	CreateActionWidget((*ActionsSource)[Index]);
}

void SActionContainer::ShowContextMenu(const FVector2D& Position)
{
	FMacroActionContextMenu::Show(
		SharedThis(this),
		Position,
		FOnActionContextButtonClicked::CreateRaw(this, &SActionContainer::ActionButtonClicked)
	);
}

void SActionContainer::CreateActionWidget(FMacroAction& Action)
{
	ActionVerticalBox->AddSlot()
	.AutoHeight()
	[
		SNew(SMacroAction)
		.Action(&Action)
		.OnDeleted(this, &SActionContainer::ActionDeleted)
	];
}

void SActionContainer::ActionButtonClicked(const SMacroActionContextButton* Button)
{
	CreateAction({
		Button->GetMacro()->GetClass(),
		Button->GetMacro()->GetDefaultParameters(),
		{}
	});
}

void SActionContainer::ActionDeleted(const TSharedRef<SMacroAction>& Action)
{
	if (!ActionsSource)
	{
		UMacroDebugStatics::PrintSimple(
			"SActionContainer::ActionDeleted - ActionsSource is invalid!",
			EMacroPrintSeverity::Warning
		);
		return;
	}
	
	// remove action from original array in macro subsystem
	int IndexToRemove {-1};
	int Index {0};
	for (auto& MacroAction : *ActionsSource)
	{
		if (&MacroAction == Action->GetAction())
		{
			IndexToRemove = Index;
			break;
		}
		
		Index++;
	}

	if (IndexToRemove != -1)
	{
		ActionsSource->RemoveAt(IndexToRemove);
	}

	// mimic the original array remove in the UI
	ActionVerticalBox->RemoveSlot(Action);
}

FReply SActionContainer::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		ShowContextMenu(MouseEvent.GetScreenSpacePosition());
		return FReply::Handled();
	}
	
	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}
