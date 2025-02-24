#pragma once

#include "ClassTypePool.h"
#include "CharacterFactoryManager.h"
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API CharacterPool : public ClassTypePool<Character, byte>
{
protected:
	Character* create(const byte type) override
	{
		return mCharacterFactoryManager->getFactory(type)->create();
	}
};