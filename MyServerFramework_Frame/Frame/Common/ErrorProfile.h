#pragma once

#include "FrameCallback.h"

// 为了避免头文件的互相包含,将errorProfile单独放到一个文件中
namespace ErrorProfile
{
	MICRO_LEGEND_FRAME_API void errorProfile(string&& log, string&& file);
	MICRO_LEGEND_FRAME_API extern ErrorProfileEvent mOnErrorProfileEvent;
};