#pragma once

#include "ClassTypePool.h"
#include "CharacterStateFactoryManager.h"

class MICRO_LEGEND_FRAME_API StatePool : public ClassTypePool<CharacterState, ushort>
{
protected:
	CharacterState* create(ushort type) override
	{
		return mCharacterStateFactoryManager->getFactory(type)->create();
	}
};