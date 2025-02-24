#include "FrameHeader.h"

RandomSystem::RandomSystem()
{
	mRandomSequence.zero();
	FOR_I(mRandomSequence.size())
	{
		mRandomSequence[i] = ((rand() & 0x7FFF) << 15) + rand();
	}
}

void RandomSystem::update(const float elapsedTime)
{
	// 每一帧更新其中一个值
	mRandomSequence[mUpdateIndex++] = ((rand() & 0x7FFF) << 15) + rand();
	if (mUpdateIndex >= mRandomSequence.size())
	{
		mUpdateIndex = 0;
	}
}