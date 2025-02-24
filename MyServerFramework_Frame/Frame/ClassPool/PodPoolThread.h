#pragma once

#include "ThreadLock.h"
#include "ThreadLockScope.h"
#include "ClassPoolBase.h"
#include "FrameMySQLUtility.h"

// pod类型的对象池
// 线程安全
template<typename T, typename TypeCheck = typename IsPodType<T>::mType>
class PodPoolThread : public ClassPoolBase
{
	BASE(PodPoolThread, ClassPoolBase);
public:
	void initDefault(const int count)
	{
		Vector<T*> list(count);
		FOR_I(count)
		{
			list.push_back(new T());
		}
		THREAD_LOCK(mLock);
		mUnusedList.addRange(list);
		mTotalCount += count;
	}
	void quit() override
	{
		THREAD_LOCK(mLock);
		for (T* obj : mUnusedList)
		{
			delete obj;
		}
		mUnusedList.clear();
	}
	T* newPod()
	{
		T* obj = nullptr;
		if (mUnusedList.size() > 0)
		{
			// 不能把LOG也锁住,LOG中也会通过对象池创建对象,会造成死锁
			THREAD_LOCK(mLock);
			// 首先从未使用的列表中获取,获取不到再重新创建一个
			obj = mUnusedList.popBack(nullptr);
		}

		// 没有找到可以用的,则创建一个
		if (obj == nullptr)
		{
			obj = new T();
			if (++mTotalCount % 5000 == 0 && mShowCountLog)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount) + "个");
			}
		}
		return obj;
	}
	void destroyPod(T*& obj)
	{
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || obj == nullptr)
		{
			return;
		}
		// 重置所有属性,一定要在添加到列表之前就重置属性,一旦添加到列表,随时就可能再分配出去,从而导致线程冲突
		THREAD_LOCK(mLock);
		// 添加到未使用列表中
		mUnusedList.push_back(obj);
		obj = nullptr;
	}
	void clearPool()
	{
		THREAD_LOCK(mLock);
		for (T* obj : mUnusedList)
		{
			delete obj;
		}
		mUnusedList.clear(true);
	}
protected:
	void onHour() override
	{
		if (mTotalCount > 1000)
		{
			LOG("ClassPoolThread: " + string(typeid(T).name()) + "的数量:" + IToS(mTotalCount));
		}
	}
protected:
	Vector<T*> mUnusedList;			// 未使用列表
	ThreadLock mLock;				// mUnusedList的锁
	int mTotalCount = 0;			// 创建的对象总数,因为不需要一个特别准确的值,所以不使用原子类型
	bool mShowCountLog = true;		// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
};