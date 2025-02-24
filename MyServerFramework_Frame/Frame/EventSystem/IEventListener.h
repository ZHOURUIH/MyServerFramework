#pragma once

#include "FrameMacro.h"

// 事件监听者接口,监听事件的类需要继承该类
class MICRO_LEGEND_FRAME_API IEventListener
{
public:
	virtual ~IEventListener() = default;
};