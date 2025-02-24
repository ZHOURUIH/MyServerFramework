#pragma once

#include "Utility.h"

class MICRO_LEGEND_FRAME_API LogFileInfo
{
public:
	void generateLogFileName()
	{
		mFileName.assign(FrameDefine::LOG_PATH + LLToS(mPlayerGUID / 100) + "/" +
			LLToS(mPlayerGUID) + "/Log_" + getTime() + ".txt");
	}
	void generateErrorFileName()
	{
		mFileName.assign(FrameDefine::LOG_PATH + LLToS(mPlayerGUID / 100) + "/" +
			LLToS(mPlayerGUID) + "/Error_" + getTime() + ".txt");
	}
public:
	string mFileName;		// 当前正在写入的日志文件名
	int mWriteLength = 0;	// 当前正在写入的日志文件已经写入的日志长度
	llong mPlayerGUID = 0;
};