#pragma once

#include "Widgets/SCompoundWidget.h"

struct FInstancedStruct;

class SLoopIterationMacro final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoopIterationMacro)
	{}
		SLATE_ARGUMENT(FInstancedStruct*, IterationData)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FInstancedStruct* IterationData {nullptr};

	[[nodiscard]] int* GetMaxIterations() const;
};
