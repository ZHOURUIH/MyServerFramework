#pragma once

#include "Utility.h"

// 单独为char实现的数组池,因为char类型使用非常频繁,所以为了避免一些不必要的类型查找,单独实现一个
class MICRO_LEGEND_FRAME_API ArrayPoolSingleChar
{
public:
	virtual ~ArrayPoolSingleChar() { destroy(); }
	void destroy();
	char* get(const int count, const bool zeroMemory, bool* isNewAlloc = nullptr)
	{
		char* data = nullptr;

		// 尝试从未使用列表中查找
		if (mUnuseMemoryList.size() > 0)
		{
			if (auto* unuseVec = mUnuseMemoryList.getPtr(count))
			{
				data = unuseVec->popBack(nullptr);
			}
		}
		if (isNewAlloc != nullptr)
		{
			*isNewAlloc = data == nullptr;
		}
		// 如果未使用列表中没有找到可以重复使用的,则创建一个
		if (data == nullptr)
		{
			data = new char[count];
			mCreatedSize += count * sizeof(char);
		}

		// 需要保证第一个元素为0
		data[0] = 0;
		if (zeroMemory)
		{
			memset(data, 0, count * sizeof(char));
		}
#ifdef WINDOWS
		mInUsedMemoryList.insertOrGet(count).insert(data);
#endif
		return data;
	}
	// 最好是在回收时能提供长度,提高效率
	void release(char* data, int size) 
	{
		if (data == nullptr || size == 0)
		{
			return;
		}
#ifdef WINDOWS
		auto* listPtr = mInUsedMemoryList.getPtr(size);
		if (listPtr == nullptr)
		{
			ERROR("回收了错误的数组");
			return;
		}
		if (!listPtr->erase(data))
		{
			ERROR("回收了错误的数组");
			return;
		}
#endif
		mUnuseMemoryList.insertOrGet(size).push_back(data);
	}
	void dump();
public:
	HashMap<int, Vector<char*>> mUnuseMemoryList;	// key是数组长度,value是该长度的所有数组
#ifdef WINDOWS
	HashMap<int, Set<char*>> mInUsedMemoryList;		// 记录分配出去的数组,key是数组长度,value是该长度的所有数组,仅在windows下调试用
#endif
	llong mCreatedSize = 0;							// 已经创建的总大小
};