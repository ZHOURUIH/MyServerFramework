#pragma once

#include "FrameComponent.h"
#include "ThreadLock.h"
#include "DoubleBuffer.h"

class MICRO_LEGEND_FRAME_API LogSystem : public FrameComponent
{
	BASE(LogSystem, FrameComponent);
public:
	LogSystem();
	void init() override;
	void quit() override;
	static void log(LogInfo* info);
protected:
	static void writeLogThread(CustomThread* thread);
	void writeLogFile();
	void writeErrorFile();
public:
	DoubleBuffer<LogInfo*> mErrorBuffer;
	DoubleBuffer<LogInfo*> mLogBuffer;
	HashMap<llong, LogFileInfo*> mErrorWriteInfoList;
	HashMap<llong, LogFileInfo*> mLogWriteInfoList;
	ThreadLock mErrorWriteInfoListLock;
	ThreadLock mLogWriteInfoListLock;
	CustomThread* mLogThread = nullptr;
	static constexpr int mMaxFileSize = 1024 * 1024;
};