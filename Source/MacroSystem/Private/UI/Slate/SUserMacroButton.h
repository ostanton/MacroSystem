#pragma once

#include "UserMacroHandle.h"
#include "Widgets/SCompoundWidget.h"

class SEditableTextBox;

DECLARE_DELEGATE_OneParam(FOnUserMacroRenamed, const TSharedRef<FUserMacroHandle>&);
DECLARE_DELEGATE_OneParam(FOnUserMacroDeleted, int);
DECLARE_DELEGATE(FOnUserMacroExecuted);

/**
 * Represents a single user macro object from the UMacroSubsystem
 */
class SUserMacroButton final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUserMacroButton)
	{}
		SLATE_ARGUMENT(TSharedPtr<FUserMacroHandle>, UserMacroHandle)
		/** Used to notify that a macro has been renamed. Its actual name is already changed at this point */
		SLATE_EVENT(FOnUserMacroRenamed, OnRenamed)
		/** Used to notify that a macro has been deleted. It has already been removed from UMacroSubsystem at this point */
		SLATE_EVENT(FOnUserMacroDeleted, OnDeleted)
		SLATE_EVENT(FOnUserMacroExecuted, OnExecuted)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void Rename();

	[[nodiscard]] const FUserMacroHandle& GetUserMacroHandle() const;
	[[nodiscard]] TSharedRef<FUserMacroHandle> GetUserMacroHandleShared() const;
	[[nodiscard]] const UMacro* GetUserMacro() const;

private:
	FSlateBrush IconBrush;
	TSharedPtr<FUserMacroHandle> UserMacroHandle;
	FOnUserMacroRenamed OnUserMacroRenamed;
	FOnUserMacroDeleted OnUserMacroDeleted;
	FOnUserMacroExecuted OnUserMacroExecuted;

	TSharedPtr<SButton> ExtrasButton;

	[[nodiscard]] bool CanRenameMacro(const FText& TargetName) const;
	void RenameTextCommitted(const FText& Text);

	void CreateContextMenu(const FVector2D& Position);
	
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
};
