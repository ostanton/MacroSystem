#include "MacroActionContextMenu.h"
#include "MacroSettings.h"
#include "FunctionLibraries/MacroDebugStatics.h"
#include "Macros/Macro.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Application/SlateApplication.h"

TSharedPtr<IMenu> FMacroActionContextMenu::Show(
	const TSharedRef<SWidget>& Parent,
	const FVector2D& Position,
	const FOnActionContextButtonClicked& OnClicked
)
{
	if (!UMacroSettings::IsValid())
	{
		UMacroDebugStatics::PrintSimple(
			"SActionContainer::ShowContextMenu - MacroSettings are invalid!",
			EMacroPrintSeverity::Error
		);
		return nullptr;
	}

	FMenuBuilder MenuBuilder {true, nullptr};
	for (const auto& [CategoryName, Category] : UMacroSettings::GetCategories())
	{
		MenuBuilder.BeginSection(NAME_None, Category.FriendlyName);

		for (const auto& MacroClassSoft : Category.MacroClasses)
		{
			auto const MacroClass = MacroClassSoft.LoadSynchronous();

			if (!MacroClass)
			{
				UMacroDebugStatics::PrintSimple(
					"SActionContainer::ShowContextMenu - MacroClass is invalid!",
					EMacroPrintSeverity::Error
				);
				break;
			}

			if (!MacroClass->ClassDefaultObject)
			{
				UMacroDebugStatics::PrintSimple(
					"SActionContainer::ShowContextMenu - MacroClass default object is invalid!",
					EMacroPrintSeverity::Error
				);
				break;
			}

			if (auto const Macro = Cast<UMacro>(MacroClass->ClassDefaultObject))
			{
				SNew(SMacroActionContextButton)
				.Macro(Macro)
				.MenuBuilder(&MenuBuilder)
				.OnClicked(OnClicked);

				continue;
			}

			UMacroDebugStatics::PrintSimple(
				"SActionContainer::ShowContextMenu - Failed to cast MacroClass default object to UMacro!",
				EMacroPrintSeverity::Error
			);
			break;
		}

		MenuBuilder.EndSection();
	}

	return FSlateApplication::Get().PushMenu(
		Parent,
		{},
		MenuBuilder.MakeWidget(),
		Position,
		FPopupTransitionEffect::ContextMenu
	);
}
