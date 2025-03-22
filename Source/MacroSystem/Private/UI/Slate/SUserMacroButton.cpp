#include "SUserMacroButton.h"
#include "SRenameMacro.h"
#include "Macros/Macro.h"
#include "Subsystems/MacroSubsystem.h"
#include "Widgets/Input/SButton.h"

void SUserMacroButton::Construct(const FArguments& InArgs)
{
	UserMacroHandle = InArgs._UserMacroHandle;
	checkf(UserMacroHandle.IsValid(), TEXT("SUserMacroButton::Construct - UserMacroHandle is invalid!"));

	OnUserMacroRenamed = InArgs._OnRenamed;
	OnUserMacroDeleted = InArgs._OnDeleted;
	OnUserMacroExecuted = InArgs._OnExecuted;
	
	auto const UserMacro = UserMacroHandle->GetMacroInfo();
	checkf(UserMacro, TEXT("SUserMacroButton::Construct - UserMacro pointer is invalid!"));
	
	IconBrush.SetResourceObject(nullptr);
	IconBrush.SetImageSize({16.f, 16.f});

	// set tooltip text
	{
		const FText Result;
		for (const auto& Action : UserMacro->Actions)
		{
			FText::Join(Result, Cast<UMacro>(Action.MacroClass->ClassDefaultObject)->GetMacroName());
		}
		SetToolTipText(Result);
	}
	
	ChildSlot
	.Padding(16.f, 4.f)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(24.f)
			.HeightOverride(24.f)
			.Content()
			[
				SNew(SImage)
				.Image(&IconBrush)
			]
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.FillWidth(1.f)
		.Padding(4.f, 0.f)
		[
			SNew(STextBlock)
			.Text(UserMacro->Name)
		]
		+SHorizontalBox::Slot()
		.Padding(4.f)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SAssignNew(ExtrasButton, SButton)
			.Text(FText::FromString("..."))
			.OnClicked_Lambda([this]
			{
				CreateContextMenu(
					ExtrasButton->GetPaintSpaceGeometry().GetAbsolutePosition() +
						FSlateApplication::Get().FindWidgetWindow(SharedThis(this))->GetPositionInScreen() +
						FVector2D {0, ExtrasButton->GetPaintSpaceGeometry().GetAbsoluteSize().Y}
				);
				return FReply::Handled();
			})
		]
	];
}

void SUserMacroButton::Rename()
{
	SRenameMacro::PushMenu(
		SharedThis(this),
		GetPaintSpaceGeometry().GetAbsolutePosition() +
			FSlateApplication::Get().FindWidgetWindow(SharedThis(this))->GetPositionInScreen() +
			FVector2D {GetPaintSpaceGeometry().GetAbsoluteSize().X, 0},
		UserMacroHandle->GetMacroInfo()->Name,
		FOnRenameMacroCommitted::CreateRaw(this, &SUserMacroButton::RenameTextCommitted)
	);
}

const FUserMacroHandle& SUserMacroButton::GetUserMacroHandle() const
{
	return *UserMacroHandle;
}

TSharedRef<FUserMacroHandle> SUserMacroButton::GetUserMacroHandleShared() const
{
	return UserMacroHandle.ToSharedRef();
}

const UMacro* SUserMacroButton::GetUserMacro() const
{
	return UserMacroHandle->GetMacro();
}

bool SUserMacroButton::CanRenameMacro(const FText& TargetName) const
{
	return !TargetName.IsEmpty();
}

void SUserMacroButton::RenameTextCommitted(const FText& Text)
{
	// only rename if the new text is not empty
	if (CanRenameMacro(Text))
	{
		UserMacroHandle->Rename(Text);
	}

	[[maybe_unused]] auto Temp = OnUserMacroRenamed.ExecuteIfBound(GetUserMacroHandleShared());
}

void SUserMacroButton::CreateContextMenu(const FVector2D& Position)
{
	FMenuBuilder MenuBuilder {true, nullptr};

	MenuBuilder.AddMenuEntry(
		FText::FromString("Execute"),
		FText::FromString("Run this macro"),
		{},
		FExecuteAction::CreateLambda([this]
		{
			[[maybe_unused]] auto Temp = OnUserMacroExecuted.ExecuteIfBound();
			UserMacroHandle->Execute();
		})
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString("Rename"),
		FText::FromString("Rename this macro to something else"),
		{},
		FExecuteAction::CreateLambda([this]
		{
			Rename();
		})
	);
	
	MenuBuilder.AddMenuEntry(
		FText::FromString("Delete"),
		FText::FromString("Remove this macro. This cannot be undone!"),
		{},
		FExecuteAction::CreateLambda([this]
		{
			const int Index {UserMacroHandle->GetIndex()};
			UserMacroHandle->RemoveAndInvalidate();
			[[maybe_unused]] auto Temp = OnUserMacroDeleted.ExecuteIfBound(Index);
		})
	);
	
	FSlateApplication::Get().PushMenu(
		SharedThis(this),
		{},
		MenuBuilder.MakeWidget(),
		Position,
		FPopupTransitionEffect::ContextMenu
	);
}

FReply SUserMacroButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		CreateContextMenu(MouseEvent.GetScreenSpacePosition());
		return FReply::Handled();
	}
	
	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}
