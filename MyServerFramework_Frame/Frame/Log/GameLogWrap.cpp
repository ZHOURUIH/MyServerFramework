#include "FrameHeader.h"

void GameLogWrap::logError(const string& info, const llong playerGUID, const bool writeToLog)
{
	if (mLogSystem == nullptr || !writeToLog || mLogInfoPool == nullptr)
	{
		print(info.c_str());
		// windows下报错就直接暂停运行
#ifdef WINDOWS
		system("pause");
#endif
		return;
	}

	LogInfo* log = mLogInfoPool->newClass();
	if (log == nullptr)
	{
		return;
	}

	log->mError = true;
	log->mStack = getStackTrace(10);
	log->mTime = getTimeSecond();
	log->mLog = getTime() + "\t| 程序错误 : " + info;
	// 太长就截断
	if (log->mLog.size() > 1524)
	{
		log->mLog = log->mLog.substr(0, 1524);
	}
	log->mPlayerGUID = playerGUID;
	print(log->mLog.c_str());
	LogSystem::log(log);
	// windows下报错就直接暂停运行
#ifdef WINDOWS
	system("pause");
#endif
}

void GameLogWrap::logInfo(const string& info, const llong playerGUID, const bool printConsole, const bool writeToLog)
{
	if (mLogSystem == nullptr || !writeToLog || mLogInfoPool == nullptr)
	{
		print(info.c_str());
		return;
	}
	LogInfo* log = mLogInfoPool->newClass();
	if (log == nullptr)
	{
		return;
	}

	log->mError = false;
	log->mTime = getTimeSecond();
	log->mLog = getTime();
	log->mLog += "\t| : ";
	log->mLog += info;
	// 太长就截断
	if (log->mLog.size() > 1524)
	{
		log->mLog = log->mLog.substr(0, 1524);
	}
	log->mPlayerGUID = playerGUID;
	if (printConsole)
	{
		print(log->mLog.c_str());
	}
	LogSystem::log(log);
}