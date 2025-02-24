#pragma once

#include "FrameBase.h"
#include "Utility.h"

template<typename ClassType>
class MICRO_LEGEND_FRAME_API VectorPool
{
public:
	static void destroy()
	{
		for (const Vector<ClassType>* ptr : mUnusedList)
		{
			delete ptr;
		}
		mUnusedList.clear();
	}
	static Vector<ClassType>* newVector()
	{
		checkMainThread();
		Vector<ClassType>* list = nullptr;

		// 首先从未使用的列表中获取,获取不到再重新创建一个
		const int unusedCount = mUnusedList.size();
		if (unusedCount > 0)
		{
			list = mUnusedList.popBack();
		}
		else
		{
			list = new Vector<ClassType>();
			++mTotalCount;
			if (mTotalCount % 5000 == 0)
			{
				LOG("分配的Vector数量已经达到:" + IToS(mTotalCount) + "个, stack:" + getStackTrace(7));
			}
		}
		return list;
	}
	static void destroyVector(Vector<ClassType>* listPtr)
	{
		// 添加到未使用列表中
		mUnusedList.push_back(listPtr);

		// 重置所有属性
		listPtr->clear();
	}
protected:
	static Vector<Vector<ClassType>*> mUnusedList;	// 未使用列表
	static int mTotalCount;
};

template<typename T>
Vector<Vector<T>*> VectorPool<T>::mUnusedList;

template<typename T>
int VectorPool<T>::mTotalCount;