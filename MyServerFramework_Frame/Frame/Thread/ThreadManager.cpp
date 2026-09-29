#include "FrameHeader.h"

ThreadManager::~ThreadManager()
{
	if (!mThreadList.isEmpty())
	{
		print(("有" + UIToS(mThreadList.size()) + "个线程未正常关闭").c_str());
	}
	// 强制关闭所有线程
	DELETE_MAP(mThreadList);
}

CustomThread* ThreadManager::createThread(const string& name, VoidCallback callback, VoidCallback preCmdCallback, VoidCallback endCmdCallback)
{
	CustomThread* thread = new CustomThread(name, callback, preCmdCallback, endCmdCallback, false);
	mThreadList.add(thread->getThreadHandle(), thread);
	return thread;
}

CustomThread* ThreadManager::createEventThread(const string& name, VoidCallback callback, VoidCallback preCmdCallback, VoidCallback endCmdCallback)
{
	// 事件线程创建后默认无限等待,第一次执行必须由wake触发。
	// 避免构造函数返回、调用者保存线程指针以前回调就提前运行。
	CustomThread* thread = new CustomThread(name, callback, preCmdCallback, endCmdCallback, true);
	mThreadList.add(thread->getThreadHandle(), thread);
	return thread;
}

void ThreadManager::destroyThread(CustomThread*& thread)
{
	if (thread == nullptr)
	{
		return;
	}
	mThreadList.remove(thread->getThreadHandle());
	DELETE(thread);
}