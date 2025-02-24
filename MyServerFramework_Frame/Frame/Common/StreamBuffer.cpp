#include "FrameHeader.h"

StreamBuffer::StreamBuffer(int bufferSize)
{
	resizeBuffer(bufferSize);
}

StreamBuffer::~StreamBuffer() 
{
	ArrayPool::deleteCharArray(mBuffer, mBufferSize);
}

bool StreamBuffer::addDataToBack(const char* data, const int count, const bool clearIfFull)
{
	// 缓冲区空闲部分足够放下数据时才处理
	if (count <= mBufferSize - (mDataStart + mDataLength))
	{
		MEMCPY(mBuffer + mDataStart + mDataLength, mBufferSize - mDataStart - mDataLength, data, count);
		mDataLength += count;
	}
	// 空闲部分不够,查看所有空闲部分是否足够,如果足够,则调整缓冲区
	else if (count <= mBufferSize - mDataLength)
	{
		MEMMOV(mBuffer, mBufferSize, mBuffer + mDataStart, mDataLength);
		mDataStart = 0;
		MEMCPY(mBuffer + mDataLength, mBufferSize - mDataLength, data, count);
		mDataLength += count;
	}
	// 整个缓冲区不足,根据参数清空缓冲区或者提示缓冲区太小
	else
	{
		if (clearIfFull && mBufferSize >= count)
		{
			clear();
			MEMCPY(mBuffer, mBufferSize, data, count);
			mDataLength += count;
		}
		else
		{
			ERROR("缓冲区太小");
		}
		return false;
	}
	return true;
}

bool StreamBuffer::addEmptyToBack(const int count, const bool clearIfFull)
{
	// 缓冲区空闲部分足够放下数据时才处理
	if (count <= mBufferSize - (mDataStart + mDataLength))
	{
		mDataLength += count;
	}
	// 空闲部分不够,查看所有空闲部分是否足够,如果足够,则调整缓冲区
	else if (count <= mBufferSize - mDataLength)
	{
		MEMMOV(mBuffer, mBufferSize, mBuffer + mDataStart, mDataLength);
		mDataStart = 0;
		mDataLength += count;
	}
	// 整个缓冲区不足,根据参数清空缓冲区或者提示缓冲区太小
	else
	{
		if (clearIfFull && mBufferSize >= count)
		{
			clear();
			mDataLength += count;
		}
		else
		{
			ERROR("缓冲区太小");
		}
		return false;
	}
	return true;
}

void StreamBuffer::resizeBuffer(const int size)
{
	if (mBufferSize >= size)
	{
		return;
	}
	
	if (mBuffer == nullptr)
	{
		mBufferSize = size;
		mBuffer = ArrayPool::newCharArray(mBufferSize);
		return;
	}

	// 创建新的缓冲区,将原来的数据拷贝到新缓冲区中,销毁原缓冲区,指向新缓冲区
	char* newBuffer = ArrayPool::newCharArray(size);
	if (mDataLength > 0)
	{
		MEMCPY(newBuffer, size, mBuffer + mDataStart, mDataLength);
	}
	ArrayPool::deleteCharArray(mBuffer, mBufferSize);
	mBufferSize = size;
	mBuffer = newBuffer;
	mDataStart = 0;
}