#pragma once

#include "LogInfo.h"
#include "ClassPoolThread.h"

class MICRO_LEGEND_FRAME_API LogInfoPool : public ClassPoolThread<LogInfo>
{
	BASE(LogInfoPool, ClassPoolThread<LogInfo>);
public:
	LogInfoPool()
	{
		mShowCountLog = false;
	}
};