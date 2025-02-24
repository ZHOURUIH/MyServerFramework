#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API GameEvent
{
public:
	ushort getType() const { return mType; }
public:
	llong mCharacterGUID = 0;
	int mType = 0;
};