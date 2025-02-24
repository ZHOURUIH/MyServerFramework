#pragma once

#include "ClassPoolBase.h"
#include "ClassPooledObject.h"
#include "Utility.h"
#include "FrameMySQLUtility.h"

// 传入的参数就是模板类型
// 仅限于在主线程使用
template<typename BaseClassType, typename TypeCheck = typename IsSubClassOf<ClassPooledObject, BaseClassType>::mType>
class ClassBaseTypePool : public ClassPoolBase
{
	BASE(ClassBaseTypePool, ClassPoolBase);
public:
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T>::mType>
	void initDefault(const int count)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
			return;
		}

		Vector<BaseClassType*> list(count);
		FOR_I(count)
		{
			BaseClassType* obj = new T();
			obj->resetProperty();
			list.push_back(obj);
		}
		mUnusedList.insertOrGet(typeid(T).hash_code()).addRange(list);
		mTotalCount.insertOrGet(typeid(T).hash_code(), make_pair(typeid(T).name(), 0)).second += count;
	}
	void quit() override
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
			return;
		}

		for (const auto& item : mUnusedList)
		{
			for (BaseClassType* obj : item.second)
			{
				delete obj;
			}
		}
		mUnusedList.clear();
	}
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T>::mType>
	void newClassList(Vector<BaseClassType*>& classList, const int dataCount)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
			return;
		}
		classList.clearAndReserve(dataCount);
		const size_t typeHash = typeid(T).hash_code();
		if (mUnusedList.size() > 0)
		{
			if (auto* listPtr = mUnusedList.getPtr(typeHash))
			{
				FOR_I(dataCount)
				{
					// 首先从未使用的列表中获取,获取不到再重新创建一个
					BaseClassType* obj = listPtr->popBack(nullptr);
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
			const int needCreateCount = dataCount - classList.size();
			FOR_I(needCreateCount)
			{
				BaseClassType* obj = new T();
				// 为了跟复用时的状态统一
				obj->resetProperty();
				classList.push_back(obj);
			}
			auto& totalCount = mTotalCount.insertOrGet(typeHash, make_pair(typeid(T).name(), 0));
			totalCount.second += needCreateCount;
			if (mShowCountLog && totalCount.second % 5000 == 0)
			{
				LOG(string(typeid(*classList[0]).name()) + "的数量已经达到了" + IToS(totalCount.second) + "个");
			}
		}
		
		for (BaseClassType* item : classList)
		{
			item->markUsable(this, ++mAssignIDSeed);
		}
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T>::mType>
	T* newClass()
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
			return nullptr;
		}

		T* obj = nullptr;
		if (mUnusedList.size() > 0)
		{
			// 首先从未使用的列表中获取,获取不到再重新创建一个
			if (auto* listPtr = mUnusedList.getPtr(typeid(T).hash_code()))
			{
				obj = static_cast<T*>(listPtr->popBack(nullptr));
			}
		}

		// 没有找到可以用的,则创建一个
		if (obj == nullptr)
		{
			obj = new T();
			obj->resetProperty();
			auto& totalCount = mTotalCount.insertOrGet(typeid(T).hash_code(), make_pair(typeid(T).name(), 0));
			++totalCount.second;
			if (mShowCountLog && totalCount.second % 5000 == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(totalCount.second) + "个");
			}
		}

		// 设置为可用状态
		obj->markUsable(this, ++mAssignIDSeed);
		return obj;
	}
	// 由于允许传入BaseClassType子类的列表,所以重新定义了一个类型
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T>::mType>
	void destroyClass(T* obj)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
			return;
		}

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || obj == nullptr)
		{
			return;
		}
		if (!obj->markDispose(this))
		{
			ERROR_PROFILE((string("0重复销毁对象:") + typeid(T).name()).c_str());
			return;
		}
		// 添加到未使用列表中
		mUnusedList.insertOrGet(typeid(*obj).hash_code()).push_back(obj);
	}
	// 由于允许传入BaseClassType子类的列表,所以重新定义了一个类型
	template<typename T, int Length, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T>::mType>
	void destroyClassArray(const Array<Length, T*>& objList, int count = -1)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
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
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("1重复销毁对象:") + typeid(T).name()).c_str());
				continue;
			}
			// 添加到未使用列表中
			mUnusedList.insertOrGet(typeid(*obj).hash_code()).push_back(obj);
		}
	}
	// 由于允许传入BaseClassType子类的列表,所以重新定义了一个类型
	template<typename T, int Length, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T>::mType>
	void destroyClassArray(const ArrayList<Length, T*>& objList)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
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
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("1重复销毁对象:") + typeid(T).name()).c_str());
				continue;
			}
			// 添加到未使用列表中
			mUnusedList.insertOrGet(typeid(*obj).hash_code()).push_back(obj);
		}
	}
	// 由于允许传入BaseClassType子类的列表,所以重新定义了一个类型
	template<typename T0, typename T1, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T1>::mType>
	void destroyClassList(const HashMap<T0, T1*>& objMap)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
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
			BaseClassType* obj = iter.second;
			if (obj == nullptr)
			{
				continue;
			}
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("2重复销毁对象:") + typeid(T1).name()).c_str());
				continue;
			}
			mUnusedList.insertOrGet(typeid(*obj).hash_code()).push_back(obj);
		}
	}
	// 由于允许传入BaseClassType子类的列表,所以重新定义了一个类型
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<BaseClassType, T>::mType>
	void destroyClassList(const Vector<T*>& objList)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(BaseClassType).name());
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
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("3重复销毁对象:") + typeid(T).name()).c_str());
				continue;
			}
			mUnusedList.insertOrGet(typeid(*obj).hash_code()).push_back(obj);
		}
	}
	//------------------------------------------------------------------------------------------------------------------
protected:
	void onHour() override
	{
		for (const auto& item : mTotalCount)
		{
			if (item.second.second > 1000)
			{
				LOG("ClassBaseTypePool: " + item.second.first + "的数量:" + IToS(item.second.second));
			}
		}
	}
protected:
	HashMap<size_t, Vector<BaseClassType*>> mUnusedList;// 未使用对象的列表
	HashMap<size_t, pair<string, int>> mTotalCount;		// 创建的对象总数
	bool mShowCountLog = true;							// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
	llong mAssignIDSeed = 0;							// 对象的分配ID种子
};