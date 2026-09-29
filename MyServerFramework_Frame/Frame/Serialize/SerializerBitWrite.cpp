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
	const uint count = (uint)list.size();
	if (!writeUnsigned(count))
	{
		return false;
	}
	bool result = true;
	FOR(count)
	{
		result = writeString(list[i]) && result;
	}
	return result;
}

void SerializerBitWrite::writeCheck(const int writeLen)
{
	// 主线程检查不需要在每次写字段时重复执行。
	// mMainThreadOnly为true时,真正涉及ArrayPool申请/释放的newChar/deleteChar内部本身会检查主线程。
	// 普通写入只操作当前Serializer自身的buffer,这里重复checkMainThread会给短消息和多字段消息增加固定开销。
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