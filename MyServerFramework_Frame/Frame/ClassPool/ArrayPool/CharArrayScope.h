#pragma once

// 为char类型数组的创建而设计的类,因为要在StringUtility.h中使用,所以不能包含其他头文件
class MICRO_LEGEND_FRAME_API CharArrayScope
{
public:
	char* mArray = nullptr;
private:
	int mSize = 0;
public:
	CharArrayScope(int count, bool zeroMemory = false);
	~CharArrayScope();
};