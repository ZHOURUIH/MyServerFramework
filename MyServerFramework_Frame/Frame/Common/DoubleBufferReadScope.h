#pragma once

#include "DoubleBuffer.h"

template<typename T>
class MICRO_LEGEND_FRAME_API DoubleBufferReadScope
{
private:
	DoubleBuffer<T>* mBuffer = nullptr;
public:
	Vector<T>* mReadList = nullptr;
public:
	DoubleBufferReadScope(DoubleBuffer<T>& buffer)
	{
		// 尝试获取一下可读列表是否有内容,没有则不再继续执行,等待下次再获取
		if (!buffer.tryCheckDataToRead())
		{
			return;
		}
		mBuffer = &buffer;
		mReadList = mBuffer->startRead();
	}
	~DoubleBufferReadScope()
	{
		if (mReadList != nullptr)
		{
			mReadList->clear();
		}
		if (mBuffer != nullptr)
		{
			mBuffer->endRead();
		}
	}
};