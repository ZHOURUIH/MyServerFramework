#pragma once

#include "ClassTypePool.h"
#include "GameComponentFactoryManager.h"
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API GameComponentPool : public ClassTypePool<GameComponent, ushort>
{
protected:
	GameComponent* create(ushort type) override
	{
		return mGameComponentFactoryManager->getFactory(type)->create();
	}
};