#pragma once

#include "FrameEnum.h"

class MICRO_LEGEND_FRAME_API KeyState
{
public:
	float mStateTime = 0.0f;				// 放开或者按下的持续时间
	KEY_STATE mState = KEY_STATE::KEEP_UP;	// 按键的状态
};