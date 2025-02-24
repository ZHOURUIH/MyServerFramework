#pragma once

#include "FactoryManager.h"

class MICRO_LEGEND_FRAME_API StateParamFactoryManager : public FactoryManager<StateParam, ushort>
{
public:
	template<typename ClassType>
	void addFactory(ushort type)
	{
		addFactoryInstance<ClassType>(type);
	}
protected:
};