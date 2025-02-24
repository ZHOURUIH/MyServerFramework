#include "FrameHeader.h"

void SerializerWrite::initCapacity(const int size)
{
	writeCheck(size);
}

SerializerWrite::~SerializerWrite()
{
	ArrayPool::deleteCharArray(mBuffer, mBufferSize);
}

bool SerializerWrite::writeStringList(const Vector<string>& list)
{
	const int count = list.size();
	if (!write(count))
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

bool SerializerWrite::writeBuffer(const char* buffer, const int bufferSize)
{
	writeCheck(bufferSize);
	return BinaryUtility::writeBuffer(mBuffer, mBufferSize, mIndex, buffer, bufferSize);
}

bool SerializerWrite::writeString(const char* str)
{
	// 先写入字符串长度
	const int writeLen = strlength(str);
	if (!write(writeLen))
	{
		return false;
	}
	writeCheck(writeLen);
	return BinaryUtility::writeBuffer(mBuffer, mBufferSize, mIndex, (char*)str, writeLen);
}

bool SerializerWrite::writeString(const string& str)
{
	// 先写入字符串长度
	const int writeLen = (int)str.length();
	if (!write(writeLen))
	{
		return false;
	}
	writeCheck(writeLen);
	return BinaryUtility::writeBuffer(mBuffer, mBufferSize, mIndex, str.c_str(), writeLen);
}

bool SerializerWrite::writeToFile(const string& fullName) const
{
	// 确保是只写的,并且数据不为空
	if (mBuffer == nullptr || mIndex == 0)
	{
		return false;
	}
	return writeFile(fullName, mBuffer, mIndex);
}

void SerializerWrite::writeCheck(const int writeLen)
{
	// 如果缓冲区为空,则创建缓冲区
	if (mBuffer == nullptr)
	{
		// 至少分配128个字节,避免初期频繁扩容
		mBufferSize = clampMin(getGreaterPower2(writeLen), 128);
		mBuffer = ArrayPool::newCharArray(mBufferSize);
		return;
	}
	// 如果缓冲区已经不够用了,则重新扩展缓冲区
	if (writeLen + mIndex > mBufferSize)
	{
		const int maxSize = getGreaterPower2(writeLen + mIndex);
		const int newSize = maxSize > mBufferSize << 1 ? maxSize : mBufferSize << 1;
		char* newBuffer = ArrayPool::newCharArray(newSize);
		MEMCPY(newBuffer, newSize, mBuffer, mBufferSize);
		ArrayPool::deleteCharArray(mBuffer, mBufferSize);
		mBuffer = newBuffer;
		mBufferSize = newSize;
	}
}