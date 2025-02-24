#include "FrameHeader.h"

void ArrayPoolSingle::destroy()
{
	// 此处也只是释放了已经回收的内存,没有回收的无法释放,所有对象池都是这样
	for (const auto& item0 : mUnuseMemoryList)
	{
		for (const auto& item1 : item0.second)
		{
			for (char* data : item1.second)
			{
				delete data;
			}
		}
	}
	mUnuseMemoryList.clear();
}

void ArrayPoolSingle::dump()
{
	for (const auto& item : mCreatedSizeList)
	{
		LOG("数组名:" + mTypeNameList.tryGet(item.first) + "总大小:" + LLToS(item.second >> 10) + "KB");
	}
}