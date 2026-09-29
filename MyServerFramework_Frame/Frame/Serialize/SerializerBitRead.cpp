#include "FrameHeader.h"

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
	uint readLen = 0;
	if (!readUnsigned(readLen))
	{
		return false;
	}
	str.resize(readLen);
	return readBuffer((char*)str.c_str(), readLen);
}

bool SerializerBitRead::readStringList(Vector<string>& list)
{
	uint count;
	if (!readUnsigned(count))
	{
		return false;
	}
	bool result = true;
	list.resize(count);
	FOR(count)
	{
		result = result && readString(list[i]);
	}
	return result;
}