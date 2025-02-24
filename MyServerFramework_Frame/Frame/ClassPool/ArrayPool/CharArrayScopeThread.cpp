#include "FrameHeader.h"

CharArrayScopeThread::CharArrayScopeThread(const int count, const bool zeroMemory)
{
	mArray = ArrayPoolThread::newCharArray(count, zeroMemory);
	mSize = count;
}

CharArrayScopeThread::~CharArrayScopeThread()
{
	ArrayPoolThread::deleteCharArray(mArray, mSize);
}