#pragma once

#include "FrameMacro.h"
#include <chrono>
#include <condition_variable>
#include <mutex>

// 单消费者自动复位线程事件。
// notify()只记录“至少有一次事件发生”,连续多次notify会合并为一次唤醒。
// 适用于网络发送线程这类:空闲时休眠,有任务时立即唤醒并一次性处理全部待办数据的场景。
class MICRO_LEGEND_FRAME_API ThreadEvent
{
public:
	ThreadEvent() = default;
	~ThreadEvent() = default;
	ThreadEvent(const ThreadEvent&) = delete;
	ThreadEvent& operator=(const ThreadEvent&) = delete;

	// 通知一个等待线程。如果当前没有线程等待,则保留信号,
	// 下一次wait/waitFor会立即返回。
	void notify()
	{
		{
			std::lock_guard<std::mutex> lock(mMutex);
			if (mSignaled)
			{
				return;
			}
			mSignaled = true;
		}
		mCondition.notify_one();
	}

	// 一直等待到收到notify。
	void wait()
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mCondition.wait(lock, [this]()
		{
			return mSignaled;
		});
		// AutoResetEvent语义:一次wait消费一次信号。
		mSignaled = false;
	}

	// 最多等待timeoutMS毫秒。
	// true:收到notify; false:超时。
	bool waitFor(const int timeoutMS)
	{
		std::unique_lock<std::mutex> lock(mMutex);
		if (!mCondition.wait_for(lock, std::chrono::milliseconds(timeoutMS), [this]()
		{
			return mSignaled;
		}))
		{
			return false;
		}
		mSignaled = false;
		return true;
	}

private:
	std::mutex mMutex;
	std::condition_variable mCondition;
	bool mSignaled = false;
};
