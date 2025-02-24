#pragma once

#include "FrameBase.h"
#include "ThreadLock.h"

class MICRO_LEGEND_FRAME_API LocaleGBKScope
{
protected:
	ThreadLock* mLock = nullptr;
	const char* mLocaleName = nullptr;
public:
	LocaleGBKScope(ThreadLock& threadLock)
	{
#ifdef LINUX
		mLock = &threadLock;
		mLock->waitForUnlock(__FILE__, __LINE__);
		mLocaleName = setlocale(LC_ALL, nullptr);
		try
		{
			setlocale(LC_ALL, LC_NAME_zh_CN_GBK);
		}
		catch (const exception& e)
		{
			ERROR(string("当前系统不支持UTF8编码:") + e.what());
		}
#endif
	}
	~LocaleGBKScope()
	{
#ifdef LINUX
		if (mLocaleName != nullptr)
		{
			setlocale(LC_ALL, mLocaleName);
		}
		mLock->unlock();
#endif
	}
};
