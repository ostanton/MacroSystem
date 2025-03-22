#include "SMacroAction.h"
#include "MacroSettings.h"
#include "SActionContainer.h"
#include "SActionParameter.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "Macros/Macro.h"
#include "Engine/Texture2D.h"
#include "Widgets/Input/SButton.h"

void SMacroAction::Construct(const FArguments& InArgs)
{
	OnMacroActionDeleted = InArgs._OnDeleted;
	OnMacroMovedUp = InArgs._OnMovedUp;
	OnMacroMovedDown = InArgs._OnMovedDown;
	MacroAction = InArgs._Action;
	checkf(MacroAction, TEXT("SMacroAction::Construct - MacroAction is invalid!"));

	SetToolTipText(MacroAction->GetDefaultMacro()->GetMacroDescription());

	IconBrush.SetResourceObject(MacroAction->GetDefaultMacro()->GetIconLoaded());

	BackgroundBrush.DrawAs = ESlateBrushDrawType::Image;
	BackgroundBrush.ImageType = ESlateBrushImageType::NoImage;
	BackgroundBrush.TintColor = FColor::White;

	const bool ValidParameters {!MacroAction->GetDefaultMacro()->GetDefaultParameters().IsEmpty()};
	const TSharedRef<SWidget> ParametersWidget = ValidParameters ?
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Parameters"))
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(ParameterVerticalBox, SVerticalBox)
		]
		:
		SNullWidget::NullWidget;

	if (MacroAction->GetDefaultMacro()->GetAllowsActions())
		ActionContainer = SNew(SActionContainer).ActionsSource(&MacroAction->Actions);
	
	const TSharedRef<SWidget> ActionContainerWidget =
		ActionContainer ? ActionContainer.ToSharedRef() : SNullWidget::NullWidget;

	const auto MacroWidget = MacroAction->GetDefaultMacro()->CreateMacroWidget(*MacroAction);

	ChildSlot
	[
		SAssignNew(RootBorder, SBorder)
		.BorderImage(&BackgroundBrush)
		.BorderBackgroundColor(UMacroSettings::GetNormalColour())
		.Padding(4.f, 4.f, 4.f, 4.f)
		.Content()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.FillWidth(0.5f)
					.VAlign(VAlign_Center)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox)
							.WidthOverride(16.f)
							.HeightOverride(16.f)
							.Content()
							[
								SNew(SImage)
								.Image(&IconBrush)
							]
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(4.f, 0.f, 0.f, 0.f)
						[
							SNew(STextBlock)
							.Text(MacroAction->GetDefaultMacro()->GetMacroName())
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(MacroWidget != SNullWidget::NullWidget ? 1.f : 0.f)
					.VAlign(VAlign_Center)
					[
						MacroWidget
					]
					+SHorizontalBox::Slot()
					.FillWidth(ValidParameters ? 1.f : 0.f)
					.VAlign(VAlign_Center)
					[
						ParametersWidget
					]
				]
				+SVerticalBox::Slot()
				.Padding(4.f)
				.AutoHeight()
				[
					ActionContainerWidget
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString("^"))
					.ToolTipText(FText::FromString("Move this action up one"))
					.OnClicked(this, &SMacroAction::MoveUpClicked)
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString("^"))
					.ToolTipText(FText::FromString("Move this action down one"))
					.OnClicked(this, &SMacroAction::MoveDownClicked)
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SAssignNew(ExtrasButton, SButton)
				.Text(FText::FromString("..."))
				.ToolTipText(FText::FromString("Edit this action"))
				.OnClicked_Lambda([this]
				{
					ShowContextMenu(
						ExtrasButton->GetPaintSpaceGeometry().GetAbsolutePosition() +
						FSlateApplication::Get().FindWidgetWindow(SharedThis(this))->GetPositionInScreen() +
						FVector2D {0.f, ExtrasButton->GetPaintSpaceGeometry().GetAbsoluteSize().Y}
					);
					return FReply::Handled();
				})
			]
		]
	];

	RefreshParameters();
	RefreshActions();
}

void SMacroAction::RefreshActions()
{
	if (ActionContainer)
		ActionContainer->Refresh(&MacroAction->Actions);
}

void SMacroAction::RefreshParameters()
{
	if (!MacroAction)
	{
		UMacroDebugStatics::PrintSimple(
			"SMacroAction::RefreshParameters - Target macro info is invalid!",
			EMacroPrintSeverity::Warning
		);
		return;
	}
	
	for (auto& Parameter : MacroAction->Parameters)
	{
		ParameterVerticalBox->AddSlot()
		[
			SNew(SActionParameter)
			.Parameter(&Parameter)
		];
	}
}

void SMacroAction::Deleted()
{
	OnMacroActionDeleted.ExecuteIfBound(SharedThis(this));
}

void SMacroAction::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);

	RootBorder->SetBorderBackgroundColor(UMacroSettings::GetHoverColour());
}

void SMacroAction::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseLeave(MouseEvent);

	RootBorder->SetBorderBackgroundColor(UMacroSettings::GetNormalColour());
}

FReply SMacroAction::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		ShowContextMenu(MouseEvent.GetScreenSpacePosition());
		return FReply::Handled();
	}
	
	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

void SMacroAction::ShowContextMenu(const FVector2D& Position)
{
	FMenuBuilder MenuBuilder {true, nullptr};
	MenuBuilder.BeginSection(NAME_None, FText::FromString("Actions"));
	MenuBuilder.AddMenuEntry(
		FText::FromString("Delete"),
		FText::FromString("Remove this action from the macro. This cannot be undone!"),
		{},
		FExecuteAction::CreateRaw(this, &SMacroAction::Deleted)
	);
	MenuBuilder.EndSection();

	FSlateApplication::Get().PushMenu(
		SharedThis(this),
		{},
		MenuBuilder.MakeWidget(),
		Position,
		FPopupTransitionEffect::ContextMenu
	);
}

FReply SMacroAction::MoveUpClicked()
{
	OnMacroMovedUp.Execute(SharedThis(this));
	return FReply::Handled();
}

FReply SMacroAction::MoveDownClicked()
{
	OnMacroMovedDown.Execute(SharedThis(this));
	return FReply::Handled();
}
