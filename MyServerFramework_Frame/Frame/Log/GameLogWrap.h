#pragma once

#include "FrameDefine.h"

// GameLog需要部分的依赖文件,所以直接包含到ServerDefine中会出现错误
// GameLogWrap不包含任何依赖文件,所以可以直接包含到ServerDefine中
class MICRO_LEGEND_FRAME_API GameLogWrap
{
public:
	// writeToLog表示是否写入到文件,一般不写入到文件的目的只是为了进入此函数打印错误信息,而不是写到文件造成可能的死锁
	static void logError(const string& info, llong playerGUID, bool writeToLog);
	static void logInfo(const string& info, llong playerGUID, bool printConsole, bool writeToLog);
};