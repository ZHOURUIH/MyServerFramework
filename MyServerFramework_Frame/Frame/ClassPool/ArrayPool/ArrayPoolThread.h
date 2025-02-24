#pragma once

#include "ArrayPoolSingle.h"
#include "ThreadLock.h"
#include "ThreadLockScope.h"
#include "Utility.h"
#include "HashMap.h"

// 仅支持申请和释放在同一线程的操作,T类型不能为需要调用构造析构的类型,释放时不会进行析构
class MICRO_LEGEND_FRAME_API ArrayPoolThread : public FrameComponent
{
	BASE(ArrayPoolThread, FrameComponent);
public:
	void init() override;
	void quit() override;
	~ArrayPoolThread() override;
	bool isClassPool() const override { return true; }
	template<typename T>
	static T* newArray(const int count, const bool zeroMemory = false)
	{
		ArrayPoolSingle* threadArrayMemory = nullptr;
		// 找到指定线程的内存列表
		const int threadID = getThreadID();
		{
			THREAD_LOCK(mLock);
			threadArrayMemory = mArrayPoolSingleList.tryGet(threadID);
			if (threadArrayMemory == nullptr)
			{
				threadArrayMemory = new ArrayPoolSingle();
				mArrayPoolSingleList.insert(threadID, threadArrayMemory);
			}
		}
		// 在线程内存列表中找到指定类型的内存列表
		return threadArrayMemory->get<T>(count, zeroMemory);
	}
	static char* newCharArray(int count, bool zeroMemory = false);
	template<typename T>
	static void deleteArray(T*& data, const int elementCount)
	{
		ArrayPoolSingle* threadArrayMemory = nullptr;
		{
			THREAD_LOCK(mLock);
			threadArrayMemory = mArrayPoolSingleList.tryGet(getThreadID());
		}
		if (threadArrayMemory != nullptr)
		{
			threadArrayMemory->release<T>(data, elementCount);
			data = nullptr;
		}
	}
	static void deleteCharArray(char*& data, int size);
private:
	static void onHour(void* userData);
protected:
	static HashMap<int, ArrayPoolSingle*> mArrayPoolSingleList;
	static HashMap<int, ArrayPoolSingleChar*> mArrayPoolSingleCharList;
	static ThreadLock mLock;
	static ThreadLock mCharLock;
};