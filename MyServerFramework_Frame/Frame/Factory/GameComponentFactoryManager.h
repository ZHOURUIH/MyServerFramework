#pragma once

#include "GameComponent.h"
#include "FactoryManager.h"

class MICRO_LEGEND_FRAME_API GameComponentFactoryManager : public FactoryManager<GameComponent, ushort>
{
public:
	template<typename ClassType, typename TypeCheck = typename IsSubClassOf<GameComponent, ClassType>::mType>
	void addFactory()
	{
		addFactoryInstance<ClassType>(ClassType::staticType());
	}
protected:
};