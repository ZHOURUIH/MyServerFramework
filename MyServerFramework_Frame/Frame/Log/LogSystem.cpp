#include "FrameHeader.h"

LogSystem::LogSystem()
{
	// 日志系统比较特殊,需要在创建时就记录下全局指针
	mLogSystem = this;
}

void LogSystem::init()
{
	mLogThread = mThreadManager->createThread("LogThread", writeLogThread, this);
	mLogThread->setTime(100);
}

void LogSystem::quit()
{
	for (const auto& item : mErrorWriteInfoList)
	{
		delete item.second;
	}
	mErrorWriteInfoList.clear();
	for (const auto& item : mLogWriteInfoList)
	{
		delete item.second;
	}
	mLogWriteInfoList.clear();
	mThreadManager->destroyThread(mLogThread);
	mLogThread = nullptr;
}

void LogSystem::writeLogFile()
{
	// 普通日志
	DoubleBufferReadScope<LogInfo*> readScope(mLogBuffer);
	if (readScope.mReadList == nullptr || readScope.mReadList->size() == 0)
	{
		return;
	}

	static HashMap<LogFileInfo*, Vector<LogInfo*>> fileInfoMap;
	for (auto& item : fileInfoMap)
	{
		item.second.clear();
	}
	{
		// 将其放入一个HashMap中,方便集中处理,避免在多个文件中来回跳
		THREAD_LOCK(mLogWriteInfoListLock);
		for (LogInfo* info : *readScope.mReadList)
		{
			LogFileInfo* fileInfo = mLogWriteInfoList.tryGet(info->mPlayerGUID);
			if (fileInfo == nullptr)
			{
				continue;
			}
			fileInfoMap.insertOrGet(fileInfo).push_back(info);
		}
	}
	// 写入文件
	string logLine;
	string logString;
	for (const auto& item : fileInfoMap)
	{
		LogFileInfo* fileInfo = item.first;
		// 生成新的日志文件名字
		if (fileInfo->mFileName.empty())
		{
			fileInfo->generateLogFileName();
		}
		logString.clear();
		for (const auto& info : item.second)
		{
			logLine = info->mLog;
			logLine += "\r\n";
			fileInfo->mWriteLength += (int)logLine.length();
			logString += logLine;
			if (fileInfo->mWriteLength >= mMaxFileSize)
			{
				writeFile(fileInfo->mFileName, logString, true);
				logString.clear();
				fileInfo->mWriteLength = 0;
				// 生成新的日志文件名字
				fileInfo->generateLogFileName();
			}
		}
		// 还有日志没写入,则继续写入剩下的日志
		if (logString.length() > 0)
		{
			writeFile(fileInfo->mFileName, logString, true);
		}
	}
	mLogInfoPool->destroyClassList(*readScope.mReadList);
}

void LogSystem::writeErrorFile()
{
	// 错误信息
	DoubleBufferReadScope<LogInfo*> readScope(mErrorBuffer);
	if (readScope.mReadList == nullptr || readScope.mReadList->size() == 0)
	{
		return;
	}

	static HashMap<LogFileInfo*, Vector<LogInfo*>> fileInfoMap;
	for (auto& item : fileInfoMap)
	{
		item.second.clear();
	}
	{
		// 将其放入一个HashMap中,方便集中处理,避免在多个文件中来回跳
		THREAD_LOCK(mErrorWriteInfoListLock);
		for (LogInfo* info : *readScope.mReadList)
		{
			LogFileInfo* fileInfo = mErrorWriteInfoList.tryGet(info->mPlayerGUID);
			if (fileInfo == nullptr)
			{
				continue;
			}
			fileInfoMap.insertOrGet(fileInfo).push_back(info);
		}
	}

	// 写入文件
	string logLine;
	string logString;
	for (const auto& item : fileInfoMap)
	{
		LogFileInfo* fileInfo = item.first;
		// 生成日志文件名字
		if (fileInfo->mFileName.empty())
		{
			fileInfo->generateErrorFileName();
		}
		logString.clear();
		for (const auto& info : item.second)
		{
			logLine = info->mLog;
			logLine += "\r\nstack:";
			logLine += info->mStack;
			fileInfo->mWriteLength += (int)logLine.length();
			logString += logLine;
			if (fileInfo->mWriteLength >= mMaxFileSize)
			{
				writeFile(fileInfo->mFileName, logString, true);
				logString.clear();
				fileInfo->mWriteLength = 0;
				// 生成日志文件名字
				fileInfo->generateErrorFileName();
			}
		}
		// 还有日志没写入,则继续写入剩下的日志
		if (logString.length() > 0)
		{
			writeFile(fileInfo->mFileName, logString, true);
		}
	}
	mLogInfoPool->destroyClassList(*readScope.mReadList);
}

void LogSystem::writeLogThread(CustomThread* thread)
{
	mLogSystem->writeLogFile();
	mLogSystem->writeErrorFile();
}

void LogSystem::log(LogInfo* info)
{
	if (info->mError)
	{
		{
			THREAD_LOCK(mLogSystem->mErrorWriteInfoListLock);
			if (!mLogSystem->mErrorWriteInfoList.contains(info->mPlayerGUID))
			{
				// 此处不使用对象池创建LogFileInfo,因为在对象池创建中也会涉及到日志的生成,会造成线程死锁
				// 而且LogFileInfo也不需要进行销毁,会一直存在
				LogFileInfo* fileInfo = new LogFileInfo();
				fileInfo->mPlayerGUID = info->mPlayerGUID;
				mLogSystem->mErrorWriteInfoList.insert(info->mPlayerGUID, fileInfo);
			}
		}
		mLogSystem->mErrorBuffer.add(info);
	}
	else
	{
		{
			THREAD_LOCK(mLogSystem->mLogWriteInfoListLock);
			if (!mLogSystem->mLogWriteInfoList.contains(info->mPlayerGUID))
			{
				LogFileInfo* fileInfo = new LogFileInfo();
				fileInfo->mPlayerGUID = info->mPlayerGUID;
				mLogSystem->mLogWriteInfoList.insert(info->mPlayerGUID, fileInfo);
			}
		}
		mLogSystem->mLogBuffer.add(info);
	}
}