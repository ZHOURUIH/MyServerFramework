#pragma once

#include "Character.h"
#include "FactoryManager.h"

class MICRO_LEGEND_FRAME_API CharacterFactoryManager : public FactoryManager<Character, ushort>
{
public:
	template<class ClassType, typename TypeCheck = typename IsSubClassOf<Character, ClassType>::mType>
	void addFactory(const ushort type)
	{
		addFactoryInstance<ClassType>(type);
	}
};