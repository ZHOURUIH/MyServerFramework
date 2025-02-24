#include "FrameHeader.h"

HashMap<int, ArrayPoolSingle*> ArrayPoolThread::mArrayPoolSingleList;
HashMap<int, ArrayPoolSingleChar*> ArrayPoolThread::mArrayPoolSingleCharList;
ThreadLock ArrayPoolThread::mLock;
ThreadLock ArrayPoolThread::mCharLock;

void ArrayPoolThread::init()
{
	mServerFramework->registeHour(this, onHour);
}

void ArrayPoolThread::quit()
{
	mServerFramework->unregisteHour(this);
}

ArrayPoolThread::~ArrayPoolThread()
{
	for (const auto& iter : mArrayPoolSingleList)
	{
		delete iter.second;
	}
	mArrayPoolSingleList.clear();
}

char* ArrayPoolThread::newCharArray(const int count, const bool zeroMemory)
{
	ArrayPoolSingleChar* threadArrayMemory = nullptr;
	// 找到指定线程的内存列表
	const int threadID = getThreadID();
	{
		THREAD_LOCK(mCharLock);
		threadArrayMemory = mArrayPoolSingleCharList.tryGet(threadID);
		if (threadArrayMemory == nullptr)
		{
			threadArrayMemory = new ArrayPoolSingleChar();
			mArrayPoolSingleCharList.insert(threadID, threadArrayMemory);
		}
	}
	// 在线程内存列表中找到指定类型的内存列表
	return threadArrayMemory->get(count, zeroMemory);
}

void ArrayPoolThread::deleteCharArray(char*& data, const int size)
{
	ArrayPoolSingleChar* threadArrayMemory = nullptr;
	{
		THREAD_LOCK(mCharLock);
		threadArrayMemory = mArrayPoolSingleCharList.tryGet(getThreadID());
	}
	if (threadArrayMemory != nullptr)
	{
		threadArrayMemory->release(data, size);
	}
	data = nullptr;
}

void ArrayPoolThread::onHour(void* userData)
{
	LOG("开始dumpArrayPoolThread");
	for (const auto& item : static_cast<This*>(userData)->mArrayPoolSingleList)
	{
		LOG("ThreadID:" + IToS(item.first));
		item.second->dump();
	}
	for (const auto& item : static_cast<This*>(userData)->mArrayPoolSingleCharList)
	{
		LOG("ThreadID:" + IToS(item.first));
		item.second->dump();
	}
	LOG("结束dumpArrayPoolThread");
}