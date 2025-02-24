#pragma once

#include "VectorPoolBase.h"
#include "ThreadLock.h"
#include "ThreadLockScope.h"

template<typename ClassType>
class VectorPoolThread : public VectorPoolBase
{
	BASE(VectorPoolThread, VectorPoolBase);
public:
	~VectorPoolThread() override { destroy(); }
	void destroy()
	{
		THREAD_LOCK(mNewLock);

		for (Vector<ClassType>* ptr : mUnusedList)
		{
			delete ptr;
		}
		mUnusedList.clear();
	}
	Vector<ClassType>* newVector()
	{
		THREAD_LOCK(mNewLock);

		Vector<ClassType>* list = nullptr;
		// 首先从未使用的列表中获取,获取不到再重新创建一个
		{
			THREAD_LOCK(mUnuseLock);
			const int unusedCount = mUnusedList.size();
			if (unusedCount > 0)
			{
				list = mUnusedList.popBack();
			}
		}

		// 没有找到可以用的,则创建一个
		if (list == nullptr)
		{
			list = new Vector<ClassType>();
			++mTotalCount;
			if (mTotalCount % 5000 == 0)
			{
				LOG("多线程中分配的Vector数量已经达到:" + IToS(mTotalCount) + "个");
			}
		}
		return list;
	}
	void destroyVector(Vector<ClassType>** listPtr)
	{
		THREAD_LOCK(mNewLock);
		Vector<ClassType>* list = *listPtr;
		*listPtr = nullptr;
		// 添加到未使用列表中
		{
			THREAD_LOCK(mUnuseLock);
			mUnusedList.push_back(list);
		}

		// 重置所有属性
		list->clear();
	}
protected:
	Vector<Vector<ClassType>*> mUnusedList;		// 未使用列表
	ThreadLock mUnuseLock;						// 未使用列表的锁
	ThreadLock mNewLock;						// 创建时的锁,创建和销毁都使用同一把锁,应该是防止重入的
	int mTotalCount = 0;
};