#include "SLoopIterationMacro.h"
#include "InstancedStruct.h"
#include "LoopIteration.h"
#include "Widgets/Input/SSpinBox.h"

void SLoopIterationMacro::Construct(const FArguments& InArgs)
{
	IterationData = InArgs._IterationData;
	checkf(IterationData, TEXT("SLoopIterationMacro::Construct - IterationData is invalid!"));

	if (!IterationData->IsValid())
	{
		IterationData->InitializeAs<FLoopIteration>(2);
	}

	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Max iterations"))
		]
		+SHorizontalBox::Slot()
		.Padding(4.f, 0.f, 0.f, 0.f)
		[
			SNew(SSpinBox<int>)
			.Value(*GetMaxIterations())
			.OnValueCommitted_Lambda([this](const int Value, ETextCommit::Type)
			{
				*GetMaxIterations() = Value;
			})
		]
	];
}

int* SLoopIterationMacro::GetMaxIterations() const
{
	return &IterationData->GetMutablePtr<FLoopIteration>()->MaxIterations;
}
