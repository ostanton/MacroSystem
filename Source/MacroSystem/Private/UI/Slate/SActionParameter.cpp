#include "SActionParameter.h"
#include "MacroParameter.h"
#include "MacroSettings.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SCheckBox.h"

void SActionParameter::Construct(const FArguments& InArgs)
{
	Parameter = InArgs._Parameter;
	checkf(Parameter, TEXT("SActionParameter::Construct - Parameter pointer is invalid!"));
	OnActionParameterValueChanged = InArgs._OnValueChanged;

	TSharedPtr<SWidget> ValueWidget;
	switch (Parameter->Type)
	{
	default:
	case EMacroParamType::String:
		ValueWidget = SNew(SEditableTextBox)
			.Text(FText::FromString(Parameter->Value))
			.OnTextCommitted(this, &SActionParameter::ParameterTextCommitted);
		break;
	case EMacroParamType::Float:
		ValueWidget = SNew(SSpinBox<float>)
			.Value(Parameter->As<float>())
			.OnValueCommitted(this, &SActionParameter::ParameterFloatCommitted);
		break;
	case EMacroParamType::Integer:
		ValueWidget = SNew(SSpinBox<int>)
			.Value(Parameter->As<int>())
			.OnValueCommitted(this, &SActionParameter::ParameterIntCommitted);
		break;
	case EMacroParamType::Boolean:
		ValueWidget = SNew(SCheckBox)
			.IsChecked(Parameter->As<bool>() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
			.OnCheckStateChanged(this, &SActionParameter::ParameterBoolStateChanged);
		break;
	}
	
	BackgroundBrush.ImageType = ESlateBrushImageType::NoImage;
	BackgroundBrush.TintColor = FColor::White;
	
	ChildSlot
	[
		SAssignNew(BackgroundBorder, SBorder)
		.BorderImage(&BackgroundBrush)
		.BorderBackgroundColor(UMacroSettings::GetNormalColour())
		.Content()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(Parameter->FriendlyName)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(Parameter->TypeAsText())
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				ValueWidget.ToSharedRef()
			]
		]
	];
}

void SActionParameter::ParameterValueChanged(const FString& Value)
{
	// since the parameter is a pointer to the original data, we just set the value here.
	// We don't need to set it on the outside
	Parameter->Value = Value;

	// suppress Rider warnings
	[[maybe_unused]] auto Temp = OnActionParameterValueChanged.ExecuteIfBound(this);
}

void SActionParameter::ParameterTextCommitted(const FText& Text, const ETextCommit::Type)
{
	ParameterValueChanged(Text.ToString());
}

void SActionParameter::ParameterFloatCommitted(const float Value, const ETextCommit::Type)
{
	ParameterValueChanged(FString::SanitizeFloat(Value));
}

void SActionParameter::ParameterIntCommitted(const int Value, const ETextCommit::Type)
{
	ParameterValueChanged(FString::FromInt(Value));
}

void SActionParameter::ParameterBoolStateChanged(const ECheckBoxState State)
{
	ParameterValueChanged(State == ECheckBoxState::Checked ? "True" : "False");
}

void SActionParameter::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);

	BackgroundBorder->SetBorderBackgroundColor(UMacroSettings::GetHoverColour());
}

void SActionParameter::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseLeave(MouseEvent);

	BackgroundBorder->SetBorderBackgroundColor(UMacroSettings::GetNormalColour());
}
