#pragma once

#include "ThreadLock.h"
#include "FrameBase.h"
#include "DoubleBuffer.h"

#include <chrono>
#include <condition_variable>
#include <mutex>

class MICRO_LEGEND_FRAME_API CustomThread
{
	THIS(CustomThread);
public:
	CustomThread(const string& name, VoidCallback callback, VoidCallback preCmdCallback = nullptr, VoidCallback endCmdCallback = nullptr, bool eventMode = false);
	virtual ~CustomThread() { destroy(); }
	void destroy();
	void setTime(int frameTimeMS, int forceSleep = 5) const;
	void wake();
	void setEventWaitTime(const int waitTimeMS)					{ mEventWaitTime = waitTimeMS; }
	int getEventWaitTime() const									{ return mEventWaitTime; }
	bool isEventMode() const										{ return mEventMode; }
	void addCmd(const Vector<DelayCommand*>& cmdInfoList)
	{
		mCmdBuffer.add(cmdInfoList);
		wakeIfEventMode();
	}
	void addCmd(DelayCommand* cmdInfo)
	{
		mCmdBuffer.add(cmdInfo);
		wakeIfEventMode();
	}
	void addCall(VoidCallback call, const char* name)
	{
		mCallBuffer.add(make_pair(call, name));
		wakeIfEventMode();
	}
	void setBackground(const bool background)				{ mIsBackground = background; }
	void setShowFPS(const bool showFPS)						{ mShowFPS = showFPS; }
	void setPrintFPS(const bool print)						{ mPrintFPS = print; }
	void setRunning(const bool running)
	{
		mRunning = running;
		if (!running && mEventMode)
		{
			mWakeCondition.notify_all();
		}
	}
	void setCmdDebug(const bool cmdDebug)					{ mCmdDebug = cmdDebug; }
	static void setShowFPSInterval(float interval)			{ mShowFPSInterval = interval; }
	MY_THREAD getThreadHandle() const						{ return mThread; }
	float getFrameTime() const								{ return mFrameTime; }
	int getThisThreadID() const								{ return mThreadID; }
protected:
	THREAD_CALLBACK_DECLEAR(run)
	{
		static_cast<This*>(args)->updateThread();
		return 0;
	}
	// 再写一个函数,避免在静态函数中只能通过变量访问成员变量
	void updateThread();
	void waitEvent();
	void wakeIfEventMode()
	{
		if (mEventMode)
		{
			wake();
		}
	}
protected:
	string mName;											// 线程名
	HashMap<string, int> mCmdTypeList;						// 用于记录每种命令有多少个,给mCmdBuffer用
	HashMap<const char*, int> mCallTypeList;				// 用于记录每种命令有多少个,给mCallBuffer用
	DoubleBuffer<DelayCommand*> mCmdBuffer;					// 命令缓冲区
	DoubleBuffer<pair<VoidCallback, const char*>> mCallBuffer;	// 延迟调用的缓冲区,其实也就是简化版的命令,效率比使用命令要高一些,为了能够调试,second是命令的名字
	VoidCallback mCallback = nullptr;						// 线程逻辑函数回调
	VoidCallback mPreCmdCallback = nullptr;					// 所有此线程中命令执行前的回调
	VoidCallback mEndCmdCallback = nullptr;					// 所有此线程中命令执行后的回调
	TimeLock* mTimeLock = nullptr;							// 线程锁帧
	MY_THREAD mThread;										// 为了兼容windows和linux,句柄不写为原子类型
	std::mutex mWakeMutex;									// 事件线程休眠/唤醒使用,不保护业务数据
	std::condition_variable mWakeCondition;					// 事件线程等待条件
	int mCurFrameCount = 0;									// 当前记录的帧数,用于计算帧率
	int mThreadID = 0;										// 线程ID
	int mFPS = 0;											// 当前的帧率
	int mPID = 0;											// linux下的pid
	float mShowFPSTime = 0.0f;								// 显示线程帧率的计时
	float mFrameTime = 0.0f;								// 这一帧所经过的时间,用于线程回调中获取更新时间用
	float mCurTime = 0.0f;									// 当前线程计时,用于计算线程帧率
	static float mShowFPSInterval;							// 每隔10秒打印一次线程帧率
	bool mIsBackground = true;								// 是否为后台线程,如果是后台线程,则在应用程序关闭时,子线程会自动强制关闭
	bool mShowFPS = true;									// 是否打印此线程的帧率到日志
	bool mPrintFPS = false;									// 是否打印此线程的帧率到控制台
	bool mCmdDebug = false;									// 是否开启此线程的命令调试
	bool mEventMode = false;								// 是否为事件驱动线程,false时保持原TimeLock轮询模式
	bool mWakePending = false;								// 是否存在一次尚未消费的唤醒,采用AutoResetEvent语义
	atomic<int> mEventWaitTime{ -1 };						// 事件线程等待时间,-1表示无限等待,>=0表示超时后执行一轮
	atomic<bool> mRunning;									// 是否正在运行
	atomic<bool> mFinish;									// 是否完成结束运行
};