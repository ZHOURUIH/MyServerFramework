#include "FrameHeader.h"

CharArrayScope::CharArrayScope(const int count, const bool zeroMemory)
{
	checkMainThread();
	mArray = ArrayPool::newCharArray(count, zeroMemory);
	mSize = count;
}

CharArrayScope::~CharArrayScope()
{
	checkMainThread();
	ArrayPool::deleteCharArray(mArray, mSize);
}