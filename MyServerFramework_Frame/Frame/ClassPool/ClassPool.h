#pragma once

#include "ClassPoolBase.h"
#include "ClassObject.h"
#include "ErrorProfile.h"
#include "SafeHashMap.h"

template<typename T>struct PoolOf { using type = void; };
// 固定类型的对象池
// 仅在主线程用
template<typename ClassType, typename TypeCheck = typename IsSubClassOf<ClassObject, ClassType>::mType>
class ClassPool : public ClassPoolBase
{
	BASE(ClassPool, ClassPoolBase);
public:
	ClassPool()
	{
		if (mInstance != nullptr)
		{
			ERROR("不能定义重复类型的对象池,type:" + string(typeid(ClassType).name()));
		}
		mInstance = this;
	}
	~ClassPool()
	{
		mInstance = nullptr;
	}
	void init() override
	{
		base::init();
	}
	void initDefault(const int count)
	{
#ifdef WINDOWS
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
#endif
		Vector<ClassType*> list(count);
		FOR(count)
		{
			ClassType* obj = new ClassType();
			obj->resetProperty();
			list.add(obj);
		}
		mUnusedList.addRange(list);
		mTotalCount += count;
	}
	void quit() override
	{
#ifdef WINDOWS
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
#endif
		DELETE_LIST(mUnusedList);
	}
	ClassType* newClass()
	{
#ifdef WINDOWS
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return nullptr;
		}
#endif
		ClassType* obj = nullptr;
		// 首先从未使用的列表中获取,获取不到再重新创建一个
		while (!mUnusedList.isEmpty() && obj == nullptr)
		{
			obj = mUnusedList.popBack(nullptr);
		}

		// 没有找到可以用的,则创建一个
		if (obj == nullptr)
		{
			obj = new ClassType();
			obj->resetProperty();
			++mTotalCount;
			if (mShowCountLog && (mTotalCount & (4096 - 1)) == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount) + "个");
			}
		}

		// 设置为可用状态
		obj->markUsable(this, ++mAssignIDSeed);
		return obj;
	}
	void destroyClass(ClassType*& obj)
	{
#ifdef WINDOWS
		if (!isMainThread())
		{
			ERROR(string("只能在主线程调用,type:") + typeid(ClassType).name());
			return;
		}
#endif
		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || obj == nullptr)
		{
			return;
		}

		obj->destroy();
		if (!obj->markDispose(this))
		{
			ERROR_PROFILE((string("0重复销毁对象:") + typeid(ClassType).name()).c_str());
			return;
		}
		// 添加到未使用列表中
		mUnusedList.add(obj);
		obj = nullptr;
	}
	void dump() override
	{
		if (mTotalCount > 1000)
		{
			LOG("ClassPool: " + string(typeid(ClassType).name()) + "的数量:" + IToS(mTotalCount) + ",总大小:" + LLToS(mTotalCount * sizeof(ClassType) / 1024) + "KB" + ", 未使用数量:" + IToS(mUnusedList.size()));
		}
	}
	static ClassPool<ClassType>* getInstance() { return mInstance; }
protected:
	Vector<ClassType*> mUnusedList;		// 未使用列表
	llong mAssignIDSeed = 0;			// 对象的分配ID种子
	int mTotalCount = 0;				// 创建的对象总数
	bool mShowCountLog = true;			// 当对象池创建总数达到一定数量时,是否打印日志信息,一般打印,但是日志的对象池不能打印
	static ClassPool<ClassType>* mInstance;
};

template<typename ClassType, typename TypeCheck>
ClassPool<ClassType>* ClassPool<ClassType, TypeCheck>::mInstance = nullptr;