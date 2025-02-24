#pragma once

#include "ArrayPoolThread.h"

template<typename T>
class ArrayScopeThread
{
public:
	T* mArray = nullptr;
private:
	int mSize = 0;
public:
	ArrayScopeThread(const int count, const bool zeroMemory = false)
	{
		mArray = ArrayPoolThread::newArray<T>(count, zeroMemory);
		mSize = count;
	}
	~ArrayScopeThread()
	{
		ArrayPoolThread::deleteArray(mArray, mSize);
	}
};