#pragma once

#include "ArrayPool.h"

template<typename T>
class ArrayScope
{
public:
	T* mArray = nullptr;
private:
	int mSize = 0;
public:
	ArrayScope(const int count, const bool zeroMemory = false)
	{
		checkMainThread();
		mArray = ArrayPool::newArray<T>(count, zeroMemory);
		mSize = count;
	}
	~ArrayScope()
	{
		checkMainThread();
		ArrayPool::deleteArray(mArray, mSize);
	}
};