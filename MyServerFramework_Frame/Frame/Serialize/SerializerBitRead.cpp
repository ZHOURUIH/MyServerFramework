#include "FrameHeader.h"

SerializerBitRead::SerializerBitRead(const char* buffer, const int bufferSize) :
	mBuffer(buffer),
	mBufferSize(bufferSize)
{}

bool SerializerBitRead::readBufferNoCopy(const char*& destBuffer, const int readLength)
{
	if (destBuffer != nullptr)
	{
		return false;
	}
	const int curByteIndex = getReadByteCount();
	if (curByteIndex + readLength > mBufferSize)
	{
		return false;
	}
	destBuffer = mBuffer + curByteIndex;
	mBitIndex = (curByteIndex + readLength) << 3;
	return true;
}

bool SerializerBitRead::readString(string& str)
{
	// 先读入字符串长度
	int readLen = 0;
	if (!readSigned(readLen))
	{
		return false;
	}
	str.resize(readLen);
	return readBuffer((char*)str.c_str(), readLen);
}

bool SerializerBitRead::readStringList(Vector<string>& list)
{
	int count;
	if (!readSigned(count))
	{
		return false;
	}
	bool result = true;
	list.resize(count);
	FOR_I(count)
	{
		result = result && readString(list[i]);
	}
	return true;
}