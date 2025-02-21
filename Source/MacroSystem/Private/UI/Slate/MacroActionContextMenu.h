#pragma once

#include "SMacroActionContextButton.h"

class IMenu;

/**
 * Static functions for creating a context menu holding all available macro actions
 */
class FMacroActionContextMenu final
{
public:
	static TSharedPtr<IMenu> Show(
		const TSharedRef<SWidget>& Parent,
		const FVector2D& Position,
		const FOnActionContextButtonClicked& OnClicked
	);
};
