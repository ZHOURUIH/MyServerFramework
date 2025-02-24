#pragma once

#include "Utility.h"

// 以线程为单位的数组池
class MICRO_LEGEND_FRAME_API ArrayPoolSingle
{
public:
	virtual ~ArrayPoolSingle() { destroy(); }
	void destroy();
	template<typename T>
	T* get(const int count, const bool zeroMemory)
	{
		const int type = (int)typeid(T).hash_code();
		void* data = nullptr;

		// 尝试从未使用列表中查找
		auto* unuseMap = mUnuseMemoryList.getPtr(type);
		if (unuseMap != nullptr && unuseMap->size() > 0)
		{
			if (auto* unuseSet = unuseMap->getPtr(count))
			{
				data = unuseSet->popBack(nullptr);
			}
		}

		// 如果未使用列表中没有找到可以重复使用的,则创建一个
		if (data == nullptr)
		{
			data = new T[count];
			// 记录下已经分配的总大小
			mTypeNameList.insert(type, typeid(T).name());
			mCreatedSizeList.insertOrGet(type) += count * sizeof(T);
		}

		// char数组和wchar_t数组需要保证第一个元素为0
		if (type == mCharType)
		{
			((char*)data)[0] = 0;
		}
		else if(type == mWCharType)
		{
			((wchar_t*)data)[0] = 0;
		}
		if (zeroMemory)
		{
			memset(data, 0, count * sizeof(T));
		}
		return static_cast<T*>(data);
	}
	template<typename T>
	void release(T* data, int elementCount)
	{
		if (data == nullptr || elementCount == 0)
		{
			return;
		}
		// 添加到未使用列表中
		mUnuseMemoryList.insertOrGet((int)typeid(T).hash_code()).insertOrGet(elementCount).push_back((char*)data);
	}
	void dump();
public:
	HashMap<int, HashMap<int, Vector<char*>>> mUnuseMemoryList;	// 第一个int是元素类型hash值,第二个int是数组长度
	HashMap<int, string> mTypeNameList;							// 类型hash值索引类型名
	HashMap<int, llong> mCreatedSizeList;						// 已经分配出去的内存大小
};