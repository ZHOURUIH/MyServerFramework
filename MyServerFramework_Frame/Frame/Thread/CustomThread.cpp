#include "FrameHeader.h"

float CustomThread::mShowFPSInterval = 10.0f;

CustomThread::CustomThread(const string& name, VoidCallback callback, VoidCallback preCmdCallback, VoidCallback endCmdCallback, const bool eventMode) :
	mName(name),
	mCallback(callback),
	mPreCmdCallback(preCmdCallback),
	mEndCmdCallback(endCmdCallback),
	mEventMode(eventMode),
	mRunning(true),
	mFinish(false)
{
	mTimeLock = new TimeLock(15, 5);
	if (mEventMode)
	{
		mShowFPS = false;
	}
	CREATE_THREAD(mThread, run, this);
#ifdef WINDOWS
	SetThreadDescription(mThread, ANSIToUnicode(mName.c_str()).c_str());
#endif
	LOG("线程启动成功:" + mName);
}

void CustomThread::destroy()
{
	if (mThread == NULL_THREAD)
	{
		return;
	}

	// 销毁已经加入到此线程但是还未执行的所有命令,但是由于无法在不同的线程获取读列表,所以此处不做操作
	// 认为每次销毁线程时都已经销毁了所有此线程中未执行的命令
	const int cmdCount = mCmdBuffer.getAllDataCount();
	if (cmdCount > 0)
	{
		INT_STR(countStr, cmdCount);
		LOG(string("销毁线程时仍然有") + countStr.str() + "个命令未销毁, 线程名:" + mName);
	}

	// 退出线程。事件线程可能正阻塞在condition_variable中,必须先唤醒。
	mRunning = false;
	if (mEventMode)
	{
		mWakeCondition.notify_all();
	}
	// 事件线程内部正在使用CustomThread成员的mutex/condition_variable,必须确认线程已经退出后才能析构。
	while ((mEventMode || !mIsBackground) && !mFinish)
	{
		SystemUtility::sleep(1);
	}
	CLOSE_THREAD(mThread);
	mCallback = nullptr;
	DELETE(mTimeLock);
	LOG("线程退出完成! 线程名:" + mName);
}

void CustomThread::setTime(const int frameTimeMS, const int forceSleep) const
{
	mTimeLock->setFrameTime(frameTimeMS);
	mTimeLock->setForceSleepTime(forceSleep);
}

void CustomThread::wake()
{
	if (!mEventMode)
	{
		return;
	}
	{
		std::lock_guard<std::mutex> lock(mWakeMutex);
		// AutoResetEvent语义:多个wake可以合并成一次,但wake发生在回调执行期间也不会丢失。
		mWakePending = true;
	}
	mWakeCondition.notify_one();
}

void CustomThread::waitEvent()
{
	std::unique_lock<std::mutex> lock(mWakeMutex);
	const int waitTimeMS = mEventWaitTime;
	if (waitTimeMS < 0)
	{
		mWakeCondition.wait(lock, [this]()
		{
			return mWakePending || !mRunning;
		});
	}
	else
	{
		mWakeCondition.wait_for(lock, std::chrono::milliseconds(waitTimeMS), [this]()
		{
			return mWakePending || !mRunning;
		});
	}
	mWakePending = false;
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
	llong lastUpdateTime = getRealTimeMS();
	while (mRunning)
	{
		if (mEventMode)
		{
			waitEvent();
			if (!mRunning)
			{
				break;
			}
			const llong curTime = getRealTimeMS();
			mFrameTime = (curTime - lastUpdateTime) * 0.001f;
			lastUpdateTime = curTime;
		}
		else
		{
			if (mTimeLock == nullptr)
			{
				break;
			}
			mFrameTime = mTimeLock->update() * 0.001f;
		}
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
			if (mShowFPSTime >= mShowFPSInterval)
			{
				mShowFPSTime -= mShowFPSInterval;
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
			CALL(mPreCmdCallback);
			DoubleBufferReadScope<DelayCommand*> readCmdScope(mCmdBuffer);
			if (readCmdScope.mReadList != nullptr)
			{
				mCmdTypeList.clear();
				const llong startTime = getRealTimeMS();
				for (DelayCommand* delayInfo : *readCmdScope.mReadList)
				{
					if (delayInfo->isDestroy())
					{
						ERROR_PROFILE("0Debug:CustomThread::updateThread发现已销毁的延迟命令");
						continue;
					}
					GameCommand* cmd = delayInfo->mCommand;
					mCmdTypeList.addOrGet(typeid(*cmd).name()) += 1;
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
				if (mCmdDebug)
				{
					const llong endTime = getRealTimeMS();
					if (!readCmdScope.mReadList->isEmpty() && endTime - startTime > 20)
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
							"毫秒,命令数量:" + UIToS(readCmdScope.mReadList->size()) +
							", 最多的是" + maxCmd +
							", 数量:" + IToS(maxCount));
					}
				}
				mDelayCommandPool->destroyClassList(*readCmdScope.mReadList);
			}

			DoubleBufferReadScope<pair<VoidCallback, const char*>> readCallScope(mCallBuffer);
			if (readCallScope.mReadList != nullptr)
			{
				mCallTypeList.clear();
				const llong startTime = getRealTimeMS();
				for (const pair<VoidCallback, const char*>& call : *readCallScope.mReadList)
				{
					mCallTypeList.addOrGet(call.second) += 1;
					CALL(call.first);
				}
				if (mCmdDebug)
				{
					const llong endTime = getRealTimeMS();
					if (!readCallScope.mReadList->isEmpty() && endTime - startTime > 20)
					{
						int maxCount = 0;
						string maxCmd;
						for (const auto& iter : mCallTypeList)
						{
							if (iter.second > maxCount)
							{
								maxCount = iter.second;
								maxCmd = iter.first;
							}
						}
						LOG("线程" + mName +
							"耗时:" + IToS((int)(endTime - startTime)) +
							"毫秒,命令数量:" + UIToS(readCallScope.mReadList->size()) +
							", 最多的是" + maxCmd +
							", 数量:" + IToS(maxCount));
					}
				}
			}
			CALL(mEndCmdCallback);
			CALL(mCallback);
		}
		catch (const exception& e)
		{
			LOG(string("捕获线程异常:") + e.what() + ", 线程名:" + mName);
		}
	}
	mFinish = true;
}