#pragma once

#include "FrameComponent.h"

// 暂时没用到
class MICRO_LEGEND_FRAME_API RandomSystem : public FrameComponent
{
	BASE(RandomSystem, FrameComponent);
public:
	RandomSystem();
	void update(const float elapsedTime) override;
	int random() 
	{
		if (mRandomSequence.size() <= mCurIndex)
		{
			mCurIndex = 0;
		}
		return mRandomSequence[mCurIndex++]; 
	}
protected:
	Array<1024 * 100, int> mRandomSequence;
	int mUpdateIndex = 0;
	int mCurIndex = 0;
};