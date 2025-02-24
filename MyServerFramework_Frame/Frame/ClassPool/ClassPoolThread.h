#pragma once

#include "ThreadLock.h"
#include "ThreadLockScope.h"
#include "ClassPoolBase.h"
#include "ClassPooledObject.h"
#include "FrameMySQLUtility.h"

// 固定类型的对象池
// 线程安全
template<typename ClassType, typename TypeCheck = typename IsSubClassOf<ClassPooledObject, ClassType>::mType>
class ClassPoolThread : public ClassPoolBase
{
	BASE(ClassPoolThread, ClassPoolBase);
public:
	ClassPoolThread() :
		mAssignIDSeed(0)
	{}
	void initDefault(const int count)
	{
		Vector<ClassType*> list(count);
		FOR_I(count)
		{
			ClassType* obj = new ClassType();
			obj->resetProperty();
			list.push_back(obj);
		}
		THREAD_LOCK(mLock);
		mUnusedList.addRange(list);
		mTotalCount += count;
	}
	void quit() override
	{
		THREAD_LOCK(mLock);
		for (ClassType* obj : mUnusedList)
		{
			delete obj;
		}
		mUnusedList.clear();
	}
	ClassType* newClassTemp()
	{
		ClassType* obj = newClass();
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
		mClassRemainTimeList.insert(obj, DEFAULT_LIFE_TIME);
#endif
		return obj;
	}
	ClassType* newClass()
	{
		ClassType* obj = nullptr;
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
			obj = new ClassType();
			obj->resetProperty();
			if (++mTotalCount % 5000 == 0 && mShowCountLog)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount) + "个");
			}
		}

		// 设置为可用状态
		obj->markUsable(this, ++mAssignIDSeed);
		return obj;
	}
	void destroyClass(ClassType* obj)
	{
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || obj == nullptr)
		{
			return;
		}

		if (!obj->markDispose(this))
		{
			ERROR_PROFILE((string("0重复销毁对象:") + typeid(ClassType).name()).c_str());
			return;
		}
#ifdef WINDOWS
		{
			THREAD_LOCK(mRemainTimeLock);
			mClassRemainTimeList.erase(obj);
		}
#endif
		// 重置所有属性,一定要在添加到列表之前就重置属性,一旦添加到列表,随时就可能再分配出去,从而导致线程冲突
		{
			THREAD_LOCK(mLock);
			// 添加到未使用列表中
			mUnusedList.push_back(obj);
		}
	}
	void destroyClassList(const Vector<ClassType*>& objList)
	{
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || objList.size() == 0)
		{
			return;
		}

		// 再添加到列表
		THREAD_LOCK(mLock);
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
#endif
		mUnusedList.reserve(mUnusedList.size() + objList.size());
		for (ClassType* obj : objList)
		{
			if (obj == nullptr)
			{
				continue;
			}
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("1重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
#ifdef WINDOWS
			mClassRemainTimeList.erase(obj);
#endif
			mUnusedList.push_back(obj);
		}
	}
	template<typename T0>
	void destroyClassList(const HashMap<T0, ClassType*>& objMap)
	{
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || objMap.size() == 0)
		{
			return;
		}

		// 添加到未使用列表中
		THREAD_LOCK(mLock);
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
#endif
		mUnusedList.reserve(mUnusedList.size() + objMap.size());
		for (const auto& objPair : objMap)
		{
			ClassType* obj = objPair.second;
			if (obj == nullptr)
			{
				continue;
			}
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("2重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
#ifdef WINDOWS
			mClassRemainTimeList.erase(obj);
#endif
			mUnusedList.push_back(obj);
		}
	}
	void clearPool()
	{
		THREAD_LOCK(mLock);
		FOR_VECTOR(mUnusedList)
		{
			delete mUnusedList[i];
		}
		mUnusedList.clear(true);
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
		mClassRemainTimeList.clear(true);
#endif
	}
protected:
	void onHour() override
	{
		if (mTotalCount > 1000)
		{
			LOG("ClassPoolThread: " + string(typeid(ClassType).name()) + "的数量:" + IToS(mTotalCount));
		}
	}
	void onSecond() override
	{
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
		for (auto& item : mClassRemainTimeList)
		{
			item.second -= 1.0f;
			if (item.second <= 0.0f)
			{
				LOG(string("ClassPoolThread: ") + typeid(*item.first).name() + ",对象内存泄漏");
			}
		}
#endif
	}
protected:
	HashMap<ClassType*, float> mClassRemainTimeList;// 创建对象的销毁倒计时,如果倒计时结束还没有被销毁,就认为忘了被销毁,需要报错提示
	Vector<ClassType*> mUnusedList;		// 未使用列表
	ThreadLock mLock;					// mUnusedList的锁
	ThreadLock mRemainTimeLock;			// mClassRemainTimeList的锁
	int mTotalCount = 0;				// 创建的对象总数,因为不需要一个特别准确的值,所以不使用原子类型
	bool mShowCountLog = true;			// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
	atomic<llong> mAssignIDSeed;		// 对象的分配ID种子
};