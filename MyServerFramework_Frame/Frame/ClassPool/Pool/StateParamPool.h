#pragma once

#include "ClassTypePool.h"
#include "StateParamFactoryManager.h"

class MICRO_LEGEND_FRAME_API StateParamPool : public ClassTypePool<StateParam, ushort>
{
protected:
	StateParam* create(const ushort type) override
	{
		return mStateParamFactoryManager->getFactory(type)->create();
	}
};