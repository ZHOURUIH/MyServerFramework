#include "FrameHeader.h"

void SerializerBitWrite::initCapacity(const int size)
{
	writeCheck(size);
}

SerializerBitWrite::~SerializerBitWrite()
{
	deleteChar(mBuffer, mBufferSize);
}

bool SerializerBitWrite::writeStringList(const Vector<string>& list)
{
	const int count = list.size();
	if (!writeSigned(count))
	{
		return false;
	}
	bool result = true;
	FOR_I(count)
	{
		result = writeString(list[i]) && result;
	}
	return result;
}

void SerializerBitWrite::writeCheck(const int writeLen)
{
	if (mMainThreadOnly)
	{
		checkMainThread();
	}
	// 如果缓冲区为空,则创建缓冲区
	if (mBuffer == nullptr)
	{
		// 至少分配128个字节,避免初期频繁扩容
		mBufferSize = clampMin(getGreaterPower2(writeLen), 128);
		mBuffer = newChar(mBufferSize);
		memset(mBuffer, 0, mBufferSize);
		return;
	}
	const int curByte = bitCountToByteCount(mBitIndex);
	// 如果缓冲区已经不够用了,则重新扩展缓冲区
	if (writeLen + curByte > mBufferSize)
	{
		const int maxSize = getGreaterPower2(writeLen + curByte);
		const int newSize = maxSize > mBufferSize << 1 ? maxSize : mBufferSize << 1;
		char* newBuffer = newChar(newSize);
		MEMCPY(newBuffer, newSize, mBuffer, mBufferSize);
		deleteChar(mBuffer, mBufferSize);
		memset(newBuffer + mBufferSize, 0, newSize - mBufferSize);
		mBuffer = newBuffer;
		mBufferSize = newSize;
	}
}

char* SerializerBitWrite::newChar(const int size) const
{
	if (mMainThreadOnly)
	{
		return ArrayPool::newCharArray(size);
	}
	else
	{
		return ArrayPoolThread::newCharArray(size);
	}
}

void SerializerBitWrite::deleteChar(char*& buffer, const int size) const
{
	if (mMainThreadOnly)
	{
		ArrayPool::deleteCharArray(buffer, size);
	}
	else
	{
		ArrayPoolThread::deleteCharArray(buffer, size);
	}
}