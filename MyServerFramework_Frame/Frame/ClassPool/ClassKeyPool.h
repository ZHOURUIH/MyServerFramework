#pragma once

#include "ClassPoolBase.h"
#include "ClassPooledObject.h"
#include "Utility.h"

// 大体上与ClassTypePool类似,ClassTypePool是根据物体的类型来创建,创建后会自动设置物体的类型
// ClassKeyPool是根据KeyType来创建,KeyType不一定是物体的类型,只是物体类型的唯一索引
// 所以跟ClassTypePool唯一的区别就是在创建物体后不会调用对象的setType
// 仅限于在主线程使用
template<typename ClassType, typename KeyType, typename TypeCheck = typename IsSubClassOf<ClassPooledObject, ClassType>::mType>
class ClassKeyPool : public ClassPoolBase
{
	BASE(ClassKeyPool, ClassPoolBase);
public:
	void quit() override
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
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
	void newClassList(const KeyType key, Vector<ClassType*>& classList, const int dataCount)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		classList.clearAndReserve(dataCount);
		if (mUnusedList.size() > 0)
		{
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
			int createCount = 0;
			const int needCreateCount = dataCount - classList.size();
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
	ClassType* newClass(const KeyType key)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return nullptr;
		}
		ClassType* obj = nullptr;
		if (mUnusedList.size() > 0)
		{
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
	T* newClass(const KeyType key)	
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return nullptr;
		}
		T* obj = nullptr;
		if (mUnusedList.size() > 0)
		{
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
	void newClassList(const KeyType key, Vector<ClassType*>& classList, const int dataCount)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		classList.clearAndReserve(dataCount);
		if (mUnusedList.size() > 0)
		{
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
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
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

		// 添加到未使用列表中
		mUnusedList.insertOrGet(key).push_back(obj);
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
				LOG("ClassKeyPool: " + item.second.first + "的数量:" + IToS(item.second.second));
			}
		}
	}
protected:
	HashMap<KeyType, Vector<ClassType*>> mUnusedList;	// 未使用对象的列表
	HashMap<KeyType, pair<string, int>> mTotalCount;	// 创建的对象总数
	bool mShowCountLog = true;							// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
	llong mAssignIDSeed = 0;							// 对象的分配ID种子
};