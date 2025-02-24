#include "FrameHeader.h"

CustomThread::CustomThread(const string& name, CustomThreadCallback callback, void* args, CustomThreadCallback preCmdCallback, CustomThreadCallback endCmdCallback) :
	mName(name),
	mCallback(callback),
	mArgs(args),
	mPreCmdCallback(preCmdCallback),
	mEndCmdCallback(endCmdCallback),
	mRunning(true),
	mFinish(true)
{
	mTimeLock = new TimeLock(15, 5);
	CREATE_THREAD(mThread, run, this);
#ifdef WINDOWS
	SetThreadDescription(mThread, ANSIToUnicode(mName.c_str()).c_str());
#endif
	LOG("线程启动成功:" + mName);
}

void CustomThread::destroy()
{
	// 销毁已经加入到此线程但是还未执行的所有命令,但是由于无法在不同的线程获取读列表,所以此处不做操作
	// 认为每次销毁线程时都已经销毁了所有此线程中未执行的命令
	const int cmdCount = mCmdBuffer.getAllDataCount();
	if (cmdCount > 0)
	{
		INT_STR(countStr, cmdCount);
		LOG(string("销毁线程时仍然有") + countStr.str() + "个命令未销毁, 线程名:" + mName);
	}

	// 退出线程
	mRunning = false;
	while (!mIsBackground && !mFinish) {}
	CLOSE_THREAD(mThread);
	mCallback = nullptr;
	delete mTimeLock;
	LOG("线程退出完成! 线程名:" + mName);
}

void CustomThread::setTime(const int frameTimeMS, const int forceSleep) const
{
	mTimeLock->setFrameTime(frameTimeMS);
	mTimeLock->setForceSleepTime(forceSleep);
}

void CustomThread::updateThread()
{
	if (mThreadID == 0)
	{
		mThreadID = getThreadID();
		mPID = getPID();
		LOG("线程ID:" + UIToS(mThreadID) + ", PID:" + UIToS(mPID) + ", 线程名:" + mName);
	}
	mFinish = false;
	while (mRunning)
	{
		if (mTimeLock == nullptr)
		{
			break;
		}
		mFrameTime = mTimeLock->update() * 0.001f;
		if (mShowFPS)
		{
			mCurTime += mFrameTime;
			++mCurFrameCount;
			if (mCurTime >= 1.0f)
			{
				mFPS = mCurFrameCount;
				mCurFrameCount = 0;
				mCurTime -= 1.0f;
			}
			mShowFPSTime += mFrameTime;
			// 每10秒记录一次线程帧率
			if (mShowFPSTime >= 10.0f)
			{
				mShowFPSTime -= 10.0f;
				const string fpsInfo = "线程:" + UIToS(mThreadID) + "-" + mName + "当前帧率:" + IToS(mFPS);
				if (mPrintFPS)
				{
					LOG(fpsInfo);
				}
				else
				{
					LOG_NO_PRINT(fpsInfo);
				}
			}
		}
		try
		{
			// 执行命令列表中的命令
			FOR_ONCE
			{
				DoubleBufferReadScope<DelayCommand*> readScope(mCmdBuffer);
				if (readScope.mReadList == nullptr)
				{
					break;
				}
				CALL(mPreCmdCallback, this);
				mCmdTypeList.clear();
				const llong startTime = getRealTimeMS();
				for (DelayCommand* delayInfo : *readScope.mReadList)
				{
					if (delayInfo->isDestroy())
					{
						ERROR_PROFILE("0Debug:CustomThread::updateThread发现已销毁的延迟命令");
						continue;
					}
					GameCommand* cmd = delayInfo->mCommand;
					mCmdTypeList.insertOrGet(typeid(*cmd).name()) += 1;
					if (delayInfo->mReceiver != nullptr)
					{
						cmd->setDelayCommand(false);
						mCommandSystem->executeCommand(cmd, delayInfo->mReceiver);
					}
					else
					{
						CommandSystem::destroyCmd(cmd);
					}
				}
				mDelayCommandPool->destroyClassList(*readScope.mReadList);

				if (mCmdDebug)
				{
					const llong endTime = getRealTimeMS();
					if (readScope.mReadList->size() > 0 && endTime - startTime > 20)
					{
						int maxCount = 0;
						string maxCmd;
						for (const auto& iter : mCmdTypeList)
						{
							if (iter.second > maxCount)
							{
								maxCount = iter.second;
								maxCmd = iter.first;
							}
						}
						LOG("线程" + mName + 
							"耗时:" + IToS((int)(endTime - startTime)) + 
							"毫秒,命令数量:" + UIToS(readScope.mReadList->size()) +
							", 最多的是" + maxCmd + 
							", 数量:" + IToS(maxCount));
					}
				}
				CALL(mEndCmdCallback, this);
			}
			CALL(mCallback, this);
		}
		catch (const exception& e)
		{
			LOG(string("捕获线程异常:") + e.what() + ", 线程名:" + mName);
		}
	}
	mFinish = true;
}