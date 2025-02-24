#pragma once

#include "FrameBase.h"

// 字节队列缓冲区,方便从尾部添加数据,从头部移除数据
class MICRO_LEGEND_FRAME_API StreamBuffer
{
public:
	explicit StreamBuffer(const int bufferSize);
	virtual ~StreamBuffer();
	bool isAvailable(const int count) const { return count <= mBufferSize - mDataLength; }
	bool addDataToBack(const char* data, int count, bool clearIfFull = false);
	bool addEmptyToBack(int count, bool clearIfFull = false);
	void removeDataFromFront(const int count)
	{
		if (mDataLength < count)
		{
			LOG("删除数据失败!数据量不足");
			return;
		}
		mDataStart += count;
		mDataLength -= count;
	}
	void clear()
	{
		mDataStart = 0;
		mDataLength = 0;
	}
	int getDataLength() const	{ return mDataLength; }
	char* getData() const		{ return mBuffer + mDataStart; }
	int getDataStart() const	{ return mDataStart; }
	int getBufferSize() const	{ return mBufferSize - mDataStart; }
protected:
	void resizeBuffer(int size);
protected:
	char* mBuffer = nullptr;
	int mBufferSize = 0;
	int mDataStart = 0;
	int mDataLength = 0;
};