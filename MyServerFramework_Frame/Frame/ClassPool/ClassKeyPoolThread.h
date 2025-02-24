#pragma once

#include "ThreadLock.h"
#include "ThreadLockScope.h"
#include "ClassPoolBase.h"
#include "ClassPooledObject.h"
#include "Utility.h"

// 大体上与ClassTypePool类似,ClassTypePool是根据物体的类型来创建,创建后会自动设置物体的类型
// ClassKeyPool是根据KeyType来创建,KeyType不一定是物体的类型,只是物体类型的唯一索引
// 所以跟ClassTypePool唯一的区别就是在创建物体后不会调用对象的setType
// 线程安全的
template<typename ClassType, typename KeyType, typename TypeCheck = typename IsSubClassOf<ClassPooledObject, ClassType>::mType>
class ClassKeyPoolThread : public ClassPoolBase
{
	BASE(ClassKeyPoolThread, ClassPoolBase);
public:
	ClassKeyPoolThread() :
		mAssignIDSeed(0)
	{}
	void quit() override
	{
		THREAD_LOCK(mLock);
		for (const auto& pair : mUnusedList)
		{
			auto& list = pair.second;
			for (auto* value : list)
			{
				delete value;
			}
		}
		mUnusedList.clear();
	}
	void newClassListTemp(const KeyType key, Vector<ClassType*>& classList, const int dataCount)
	{
		newClassList(key, classList, dataCount);
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
		for (ClassType* obj : classList)
		{
			mClassRemainTimeList.insert(obj, DEFAULT_LIFE_TIME);
		}
#endif
	}
	void newClassList(const KeyType key, Vector<ClassType*>& classList, const int dataCount)
	{
		classList.clearAndReserve(dataCount);
		if (mUnusedList.size() > 0)
		{
			// 不要将LOG也一起锁住,LOG也会从对象池创建对象,会造成死锁
			THREAD_LOCK(mLock);
			if (auto* objSet = mUnusedList.getPtr(key))
			{
				FOR_I(dataCount)
				{
					// 首先从未使用的列表中获取,获取不到再重新创建一个
					ClassType* obj = objSet->popBack(nullptr);
					if (obj == nullptr)
					{
						break;
					}
					classList.push_back(obj);
				}
			}
		}

		if (classList.size() < dataCount)
		{
			// 如果还没有从对象池中获取够足够数量,则重新创建
			ClassType* tempValidObj = nullptr;
			const int needCreateCount = dataCount - classList.size();
			int createCount = 0;
			FOR_I(needCreateCount)
			{
				ClassType* obj = create(key);
				// 这里可能会创建失败
				if (obj != nullptr)
				{
					++createCount;
					// 这里其实不一定需要再调用一次resetProperty,只是为了跟复用时统一
					obj->resetProperty();
					tempValidObj = obj;
				}
				classList.push_back(obj);
			}
			THREAD_LOCK(mTotalCountLock);
			mTotalCount.insertOrGet(key, make_pair(typeid(*tempValidObj).name(), 0)).second += createCount;
			if (mShowCountLog && mTotalCount[key].second % 5000 == 0 && tempValidObj != nullptr)
			{
				LOG(string(typeid(*tempValidObj).name()) + "的数量已经达到了" + IToS(mTotalCount[key].second) + "个");
			}
		}

		for (ClassType* item : classList)
		{
			if (item != nullptr)
			{
				item->markUsable(this, ++mAssignIDSeed);
			}
		}
	}
	ClassType* newClassTemp(const KeyType key)
	{
		ClassType* obj = newClass(key);
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
		mClassRemainTimeList.insert(obj, DEFAULT_LIFE_TIME);
#endif
		return obj;
	}
	ClassType* newClass(const KeyType key)
	{
		ClassType* obj = nullptr;
		if (mUnusedList.size() > 0)
		{
			THREAD_LOCK(mLock);
			// 首先从未使用的列表中获取,获取不到再重新创建一个
			if (auto* objSet = mUnusedList.getPtr(key))
			{
				obj = objSet->popBack(nullptr);
			}
		}

		// 没有找到可以用的,则创建一个
		if (obj == nullptr)
		{
			obj = create(key);
			if (obj == nullptr)
			{
				return nullptr;
			}
			obj->resetProperty();
			THREAD_LOCK(mTotalCountLock);
			++mTotalCount.insertOrGet(key, make_pair(typeid(*obj).name(), 0)).second;
			if (mShowCountLog && mTotalCount[key].second % 5000 == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount[key].second) + "个");
			}
		}
		// 设置为可用状态
		if (obj != nullptr)
		{
			obj->markUsable(this, ++mAssignIDSeed);
		}
		return obj;
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	T* newClassTemp(const KeyType key)
	{
		T* obj = newClass<T>(key);
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
		mClassRemainTimeList.insert(obj, DEFAULT_LIFE_TIME);
#endif
		return obj;
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	T* newClass(const KeyType key)	
	{
		T* obj = nullptr;
		if (mUnusedList.size() > 0)
		{
			THREAD_LOCK(mLock);
			// 首先从未使用的列表中获取,获取不到再重新创建一个
			if (auto* objVector = mUnusedList.getPtr(key))
			{
				obj = static_cast<T*>(objVector->popBack(nullptr));
			}
		}
		// 没有找到可以用的,则创建一个
		if (obj == nullptr)
		{
			obj = new T();
			obj->resetProperty();
			THREAD_LOCK(mTotalCountLock);
			++mTotalCount.insertOrGet(key, make_pair(typeid(*obj).name(), 0)).second;
			if (mShowCountLog && mTotalCount[key].second % 5000 == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount[key].second) + "个");
			}
		}

		// 设置为可用状态
		obj->markUsable(this, ++mAssignIDSeed);
		return obj;
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void newClassListTemp(const KeyType key, Vector<ClassType*>& classList, const int dataCount)
	{
		newClassList<T>(key, classList, dataCount);
#ifdef WINDOWS
		THREAD_LOCK(mRemainTimeLock);
		for (T* obj : classList)
		{
			mClassRemainTimeList.insert(obj, DEFAULT_LIFE_TIME);
		}
#endif
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void newClassList(const KeyType key, Vector<ClassType*>& classList, const int dataCount)
	{
		classList.clearAndReserve(dataCount);
		if (mUnusedList.size() > 0)
		{
			THREAD_LOCK(mLock);
			if (auto* objVector = mUnusedList.getPtr(key))
			{
				FOR_I(dataCount)
				{
					// 首先从未使用的列表中获取,获取不到再重新创建一个
					T* obj = static_cast<T*>(objVector->popBack(nullptr));
					if (obj == nullptr)
					{
						break;
					}
					classList.push_back(obj);
				}
			}
		}

		if (classList.size() < dataCount)
		{
			// 还没有获取够,则创建剩余数量
			const int needCreateCount = dataCount - classList.size();
			FOR_I(needCreateCount)
			{
				T* obj = new T();
				obj->resetProperty();
				classList.push_back(obj);
			}
			THREAD_LOCK(mTotalCountLock);
			mTotalCount.insertOrGet(key, make_pair(typeid(*classList[0]).name(), 0)).second += needCreateCount;
			if (mShowCountLog && mTotalCount[key].second % 5000 == 0)
			{
				LOG(string(typeid(*classList[0]).name()) + "的数量已经达到了" + IToS(mTotalCount[key].second) + "个");
			}
		}
		
		// 设置为可用状态
		for (ClassType* item : classList)
		{
			item->markUsable(this, ++mAssignIDSeed);
		}
	}
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClass(T*& obj, const KeyType key)
	{
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || obj == nullptr)
		{
			return;
		}

		// 先重置所有属性,确认设置为已回收,再添加到列表,一旦添加到列表,随时就可能再分配出去,最后加入列表可以避免线程冲突
		if (!obj->markDispose(this))
		{
			ERROR_PROFILE((string("0重复销毁对象:") + typeid(ClassType).name()).c_str());
			return;
		}

		{
			THREAD_LOCK(mLock);
			// 添加到未使用列表中
			mUnusedList.insertOrGet(key).push_back(obj);
		}
#ifdef WINDOWS
		{
			THREAD_LOCK(mRemainTimeLock);
			mClassRemainTimeList.erase(obj);
		}
#endif
		obj = nullptr;
	}
	//------------------------------------------------------------------------------------------------------------------
protected:
	virtual ClassType* create(const KeyType key) = 0;
	void onHour() override
	{
		for (const auto& item : mTotalCount)
		{
			if (item.second.second > 1000)
			{
				LOG("ClassKeyPoolThread: " + item.second.first + "的数量:" + IToS(item.second.second));
			}
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
				LOG(string("ClassKeyPoolThread: ") + typeid(*item.first).name() + ",对象内存泄漏");
			}
		}
#endif
	}
protected:
	HashMap<KeyType, Vector<ClassType*>> mUnusedList;	// 未使用对象的列表
	HashMap<KeyType, pair<string, int>> mTotalCount;	// 创建的对象总数
	HashMap<ClassType*, float> mClassRemainTimeList;	// 创建对象的销毁倒计时,如果倒计时结束还没有被销毁,就认为忘了被销毁,需要报错提示
	ThreadLock mLock;									// mUnusedList的锁
	ThreadLock mTotalCountLock;							// mTotalCount的锁
	ThreadLock mRemainTimeLock;							// mClassRemainTimeList的锁
	bool mShowCountLog = true;							// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
	atomic<llong> mAssignIDSeed;						// 对象的分配ID种子
};