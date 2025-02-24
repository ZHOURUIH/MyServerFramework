#pragma once

#include "ClassPoolBase.h"
#include "ClassPooledObject.h"
#include "Utility.h"
#include "FrameMySQLUtility.h"

// 根据key创建对应类型的对象,会调用对象的setType
// 仅限主线程用的
template<typename ClassType, typename KeyType, typename TypeCheck = typename IsSubClassOf<ClassPooledObject, ClassType>::mType>
class ClassTypePool : public ClassPoolBase
{
	BASE(ClassTypePool, ClassPoolBase);
public:
	void initDefault(const KeyType type, const int count)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		if (count == 0)
		{
			return;
		}
		Vector<ClassType*> list(count);
		FOR_I(count)
		{
			ClassType* obj = create(type);
			obj->resetProperty();
			list.push_back(obj);
		}
		mTotalCount.insertOrGet(type, make_pair(typeid(*list[0]).name(), 0)).second += count;
		mUnusedList.insertOrGet(type).addRange(move(list));
	}
	void quit() override
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		for (const auto& item : mUnusedList)
		{
			auto& list = item.second;
			for (ClassType* obj : list)
			{
				delete obj;
			}
		}
		mUnusedList.clear();
	}
	void newClassList(const KeyType type, Vector<ClassType*>& classList, const int dataCount)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		classList.clearAndReserve(dataCount);
		if (mUnusedList.size() > 0)
		{
			auto* objSet = mUnusedList.getPtr(type);
			if (objSet != nullptr && objSet->size() > 0)
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
			ClassType* tempValidObj = nullptr;
			const int needCreateCount = dataCount - classList.size();
			int createCount = 0;
			FOR_I(needCreateCount)
			{
				// 此处可能会创建失败
				ClassType* obj = create(type);
				if (obj != nullptr)
				{
					++createCount;
					// 为了跟复用时的状态统一
					obj->resetProperty();
					tempValidObj = obj;
				}
				classList.push_back(obj);
			}
			mTotalCount.insertOrGet(type, make_pair(typeid(*tempValidObj).name(), 0)).second += createCount;
			if (mShowCountLog && mTotalCount[type].second % 5000 == 0 && tempValidObj != nullptr)
			{
				LOG(string(typeid(*tempValidObj).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}
		
		for (ClassType* item : classList)
		{
			if (item == nullptr)
			{
				continue;
			}
			item->markUsable(this, ++mAssignIDSeed);
			item->setType(type);
		}
	}
	ClassType* newClass(const KeyType type)
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
			if (auto* objSet = mUnusedList.getPtr(type))
			{
				obj = objSet->popBack(nullptr);
			}
		}

		// 没有找到可以用的,则创建一个
		if (obj == nullptr)
		{
			obj = create(type);
			if (obj == nullptr)
			{
				return nullptr;
			}
			obj->resetProperty();
			++mTotalCount.insertOrGet(type, make_pair(typeid(*obj).name(), 0)).second;
			if (mShowCountLog && mTotalCount[type].second % 5000 == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}
		// 设置为可用状态
		obj->markUsable(this, ++mAssignIDSeed);
		obj->setType(type);
		return obj;
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	T* newClass(const KeyType type)
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
			if (auto* objVector = mUnusedList.getPtr(type))
			{
				obj = static_cast<T*>(objVector->popBack(nullptr));
			}
		}

		// 没有找到可以用的,则创建一个
		if (obj == nullptr)
		{
			obj = new T();
			obj->resetProperty();
			++mTotalCount.insertOrGet(type, make_pair(typeid(*obj).name(), 0)).second;
			if (mShowCountLog && mTotalCount[type].second % 5000 == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}

		// 设置为可用状态
		obj->markUsable(this, ++mAssignIDSeed);
		obj->setType(type);
		return obj;
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void newClassList(const KeyType type, Vector<ClassType*>& classList, const int dataCount)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		classList.clearAndReserve(dataCount);
		if (mUnusedList.size() > 0)
		{
			FOR_I(dataCount)
			{
				T* obj = nullptr;
				// 首先从未使用的列表中获取,获取不到再重新创建一个
				if (auto* objVector = mUnusedList.getPtr(type))
				{
					obj = static_cast<T*>(objVector->popBack(nullptr));
				}
				if (obj == nullptr)
				{
					break;
				}
				classList.push_back(obj);
			}
		}
		if (classList.size() < dataCount)
		{
			const int needCreateCount = dataCount - classList.size();
			FOR_I(needCreateCount)
			{
				T* obj = new T();
				obj->resetProperty();
				classList.push_back(obj);
			}
			mTotalCount.insertOrGet(type, make_pair(typeid(*classList[0]).name(), 0)).second += needCreateCount;
			if (mShowCountLog && mTotalCount[type].second % 5000 == 0)
			{
				LOG(string(typeid(*classList[0]).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}

		// 设置为可用状态
		for (ClassType* item : classList)
		{
			item->markUsable(this, ++mAssignIDSeed);
			item->setType(type);
		}
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClass(T* obj)
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
		
		// 重置属性之前预先获取类型
		const KeyType key = obj->getType();
		if (!obj->markDispose(this))
		{
			ERROR_PROFILE((string("0重复销毁对象:") + typeid(ClassType).name()).c_str());
			return;
		}
		// 添加到未使用列表中
		mUnusedList.insertOrGet(key).push_back(obj);
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<typename T, int Length, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClassArray(const Array<Length, T*>& objList, int count = -1)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (count == 0 || mDestroied)
		{
			return;
		}
		if (count < 0)
		{
			count = Length;
		}

		// 添加到列表,并重置属性
		FOR_I(count)
		{
			T* obj = objList[i];
			if (obj == nullptr)
			{
				continue;
			}
			// 重置属性之前预先获得类型
			const KeyType key = obj->getType();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("1重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			// 添加到未使用列表中
			mUnusedList.insertOrGet(key).push_back(obj);
		}
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<typename T, int Length, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClassArray(const ArrayList<Length, T*>& objList)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		const int count = objList.size();
		if (count == 0 || mDestroied)
		{
			return;
		}
		// 添加到列表,并重置属性
		FOR_I(count)
		{
			T* obj = objList[i];
			if (obj == nullptr)
			{
				continue;
			}
			// 重置属性之前预先获得类型
			const KeyType key = obj->getType();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("1重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			// 添加到未使用列表中
			mUnusedList.insertOrGet(key).push_back(obj);
		}
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<typename T0, typename T1, typename TypeCheck0 = typename IsSubClassOf<ClassType, T1>::mType>
	void destroyClassList(const HashMap<T0, T1*>& objMap)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (objMap.size() == 0 || mDestroied)
		{
			return;
		}

		// 加入到列表,并重置所有属性
		for (const auto& iter : objMap)
		{
			ClassType* obj = iter.second;
			if (obj == nullptr)
			{
				continue;
			}
			// 重置属性之前预先获得类型
			const KeyType key = obj->getType();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("2重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			mUnusedList.insertOrGet(key).push_back(obj);
		}
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClassList(const Vector<T*>& objList)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (objList.size() == 0 || mDestroied)
		{
			return;
		}

		// 加入到列表,并重置所有属性
		for (T* obj : objList)
		{
			if (obj == nullptr)
			{
				continue;
			}
			const KeyType key = obj->getType();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("3重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			mUnusedList.insertOrGet(key).push_back(obj);
		}
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<int Length, typename T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClassList(const ArrayList<Length, T*>& objList)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (objList.size() == 0 || mDestroied)
		{
			return;
		}

		// 加入到列表,并重置所有属性
		FOR_I(objList.size())
		{
			T* obj = objList[i];
			if (obj == nullptr)
			{
				continue;
			}
			const KeyType key = obj->getType();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("3重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			mUnusedList.insertOrGet(key).push_back(obj);
		}
	}
	int getTotalCount(const KeyType key) const { return mTotalCount.tryGet(key).second; }
	//------------------------------------------------------------------------------------------------------------------
protected:
	virtual ClassType* create(const KeyType type) = 0;
	void onHour() override
	{
		for (const auto& item : mTotalCount)
		{
			if (item.second.second > 1000)
			{
				LOG("ClassTypePool: " + item.second.first + "的数量:" + IToS(item.second.second));
			}
		}
	}
protected:
	HashMap<KeyType, Vector<ClassType*>> mUnusedList;	// 未使用对象的列表
	HashMap<KeyType, pair<string, int>> mTotalCount;	// 创建的对象总数
	llong mAssignIDSeed = 0;							// 对象的分配ID种子
	bool mShowCountLog = true;							// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
};