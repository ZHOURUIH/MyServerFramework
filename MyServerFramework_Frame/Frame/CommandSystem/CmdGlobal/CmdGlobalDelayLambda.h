#pragma once

#include "GameCommand.h"
#include "FrameCallback.h"

// 用于延迟执行一个lambda表达式
class MICRO_LEGEND_FRAME_API CmdGlobalDelayLambda : public GameCommand
{
	BASE(CmdGlobalDelayLambda, GameCommand);
public:
	void reset() override
	{
		mFunction = nullptr;
	}
	void execute() override { CALL(mFunction); }
public:
	VoidCallback mFunction;
};