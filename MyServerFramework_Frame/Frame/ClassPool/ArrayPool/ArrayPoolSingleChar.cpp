#include "FrameHeader.h"

void ArrayPoolSingleChar::destroy()
{
	// 此处也只是释放了已经回收的内存,没有回收的无法释放,所有对象池都是这样
	for (const auto& item0 : mUnuseMemoryList)
	{
		for (char* data : item0.second)
		{
			delete data;
		}
	}
	mUnuseMemoryList.clear();
}

void ArrayPoolSingleChar::dump()
{
	LOG("数组名char:总大小:" + LLToS(mCreatedSize >> 10) + "KB");
	for (const auto& item : mUnuseMemoryList)
	{
		LOG("Unused数组长度:" + IToS(item.first) + ", 数量:" + IToS(item.second.size()));
	}
}