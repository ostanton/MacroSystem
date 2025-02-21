#include "SMacroActionContextButton.h"
#include "Macros/Macro.h"
#include "Engine/Texture2D.h"

void SMacroActionContextButton::Construct(const FArguments& InArgs)
{
	checkf(InArgs._MenuBuilder, TEXT("SMacroActionContextButton::Construct - Invalid menu builder pointer!"));
	
	Macro = InArgs._Macro;
	checkf(Macro, TEXT("SMacroActionContextButton::Construct - Invalid macro pointer!"));
	OnClicked = InArgs._OnClicked;
	IconBrush.SetResourceObject(Macro->GetIconLoaded());
	IconBrush.SetImageSize(FVector2D {16, 16});
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SImage)
			.Image(&IconBrush)
		]
		+SHorizontalBox::Slot()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(Macro->GetMacroName())
		]
	];

	InArgs._MenuBuilder->AddMenuEntry(
		FExecuteAction::CreateLambda([this]
		{
			OnClicked.Execute(this);
		}),
		SharedThis(this),
		NAME_None,
		Macro->GetMacroDescription()
	);
}

UMacro* SMacroActionContextButton::GetMacro() const
{
	return Macro;
}
