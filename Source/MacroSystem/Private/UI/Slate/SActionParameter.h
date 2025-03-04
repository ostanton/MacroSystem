#pragma once

#include "Widgets/SCompoundWidget.h"

struct FMacroParameter;
class SActionParameter;
enum class ECheckBoxState : uint8;

DECLARE_DELEGATE_OneParam(FOnActionParameterValueChanged, const SActionParameter*);

/**
 * A single parameter for an action inside a user macro. Essentially just a wrapper (with visual representation) for
 * a user macro's action parameter
 */
class SActionParameter final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SActionParameter)
		: _Parameter(nullptr)
	{}
		SLATE_ARGUMENT(FMacroParameter*, Parameter)
		SLATE_EVENT(FOnActionParameterValueChanged, OnValueChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	[[nodiscard]] FMacroParameter* GetParameter() const {return Parameter;}

	FOnActionParameterValueChanged OnActionParameterValueChanged;

private:
	FMacroParameter* Parameter {nullptr};

	void ParameterValueChanged(const FString& Value);
	void ParameterTextCommitted(const FText& Text, ETextCommit::Type);
	void ParameterFloatCommitted(float Value, ETextCommit::Type);
	void ParameterIntCommitted(int Value, ETextCommit::Type);
	void ParameterBoolStateChanged(ECheckBoxState State);

	TSharedPtr<SBorder> BackgroundBorder;
	FSlateBrush BackgroundBrush;
	
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
};
