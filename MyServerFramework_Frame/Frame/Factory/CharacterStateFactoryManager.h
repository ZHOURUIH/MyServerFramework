#pragma once

#include "FactoryManager.h"

class MICRO_LEGEND_FRAME_API CharacterStateFactoryManager : public FactoryManager<CharacterState, ushort>
{
public:
	template<typename ClassType>
	void addFactory(const ushort type)
	{
		addFactoryInstance<ClassType>(type);
	}
protected:
};