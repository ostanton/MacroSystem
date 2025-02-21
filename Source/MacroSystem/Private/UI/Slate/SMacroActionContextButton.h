#pragma once

#include "Widgets/SCompoundWidget.h"

struct FMacroAction;
class SMacroActionContextButton;
class UMacro;
class FMenuBuilder;

DECLARE_DELEGATE_OneParam(FOnActionContextButtonClicked, const SMacroActionContextButton*);

/**
 * Wrapper widget to allow more information to be passed around when clicking an auto-generated button in
 * the macro editor's action context menu
 */
class SMacroActionContextButton final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMacroActionContextButton)
		: _Macro(nullptr)
		, _MenuBuilder(nullptr)
	{}
		SLATE_ARGUMENT(UMacro*, Macro)
		SLATE_ARGUMENT(FMenuBuilder*, MenuBuilder)
		SLATE_EVENT(FOnActionContextButtonClicked, OnClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	[[nodiscard]] UMacro* GetMacro() const;

private:
	FOnActionContextButtonClicked OnClicked;
	UMacro* Macro {nullptr};
	FSlateBrush IconBrush;
};
