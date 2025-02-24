#pragma once

#include "ClassPoolBase.h"
#include "ClassPooledObject.h"
#include "FrameMySQLUtility.h"

// 固定类型的对象池
// 仅在主线程用
template<typename ClassType, typename TypeCheck = typename IsSubClassOf<ClassPooledObject, ClassType>::mType>
class ClassPool : public ClassPoolBase
{
	BASE(ClassPool, ClassPoolBase);
public:
	void initDefault(const int count)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		Vector<ClassType*> list(count);
		FOR_I(count)
		{
			ClassType* obj = new ClassType();
			obj->resetProperty();
			list.push_back(obj);
		}
		mUnusedList.addRange(list);
		mTotalCount += count;
	}
	void quit() override
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		for (ClassType* obj : mUnusedList)
		{
			delete obj;
		}
		mUnusedList.clear();
	}
	ClassType* newClass()
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

		if (!obj->markDispose(this))
		{
			ERROR_PROFILE((string("0重复销毁对象:") + typeid(ClassType).name()).c_str());
			return;
		}
		// 添加到未使用列表中
		mUnusedList.push_back(obj);
	}
	void destroyClassList(const Vector<ClassType*>& objList)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || objList.size() == 0)
		{
			return;
		}
		
		// 再添加到列表
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
			mUnusedList.push_back(obj);
		}
	}
	template<int Length>
	void destroyClassList(const ArrayList<Length, ClassType*>& objList)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || objList.size() == 0)
		{
			return;
		}

		// 再添加到列表
		mUnusedList.reserve(mUnusedList.size() + objList.size());
		FOR_I(objList.size())
		{
			ClassType* obj = objList[i];
			if (obj == nullptr)
			{
				continue;
			}
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("1重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			mUnusedList.push_back(obj);
		}
	}
	template<typename T0>
	void destroyClassList(const HashMap<T0, ClassType*>& objMap)
	{
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || objMap.size() == 0)
		{
			return;
		}

		// 添加到未使用列表中
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
			mUnusedList.push_back(obj);
		}
	}
protected:
	void onHour() override
	{
		if (mTotalCount > 1000)
		{
			LOG("ClassPool: " + string(typeid(ClassType).name()) + "的数量:" + IToS(mTotalCount));
		}
	}
protected:
	Vector<ClassType*> mUnusedList;		// 未使用列表
	llong mAssignIDSeed = 0;			// 对象的分配ID种子
	int mTotalCount = 0;				// 创建的对象总数
	bool mShowCountLog = true;			// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
};