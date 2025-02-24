#pragma once

// 为char类型数组的创建而设计的可在子线程中使用的类,因为要在StringUtility.h中使用,所以不能包含其他头文件
class MICRO_LEGEND_FRAME_API CharArrayScopeThread
{
public:
	char* mArray = nullptr;
private:
	int mSize = 0;
public:
	CharArrayScopeThread(int count, bool zeroMemory = false);
	~CharArrayScopeThread();
};