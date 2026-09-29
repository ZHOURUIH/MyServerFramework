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
	if (count <= 0)
	{
		return true;
	}

	int freeSpace = mBufferSize - mDataLength;
	if (count > freeSpace)
	{
		if (clearIfFull && mBufferSize >= count)
		{
			clear();
			freeSpace = mBufferSize;
		}
		else
		{
			ERROR("缓冲区太小");
			return false;
		}
	}

	// 环形写入：可能分两段拷贝
	const int firstLen = getMin(count, mBufferSize - mTail);
	MEMCPY(mBuffer + mTail, mBufferSize - mTail, data, firstLen);

	if (firstLen < count)
	{
		// 跨越缓冲区末尾，第二段从头部开始
		MEMCPY(mBuffer, mBufferSize, data + firstLen, count - firstLen);
	}

	mTail = (mTail + count) % mBufferSize;
	mDataLength += count;
	return true;
}

bool StreamBuffer::addEmptyToBack(const int count, const bool clearIfFull)
{
	if (count <= 0)
	{
		return true;
	}

	int freeSpace = mBufferSize - mDataLength;
	if (count > freeSpace)
	{
		if (clearIfFull && mBufferSize >= count)
		{
			clear();
			freeSpace = mBufferSize;
		}
		else
		{
			ERROR("缓冲区太小");
			return false;
		}
	}

	mTail = (mTail + count) % mBufferSize;
	mDataLength += count;
	return true;
}

int StreamBuffer::peekData(void* dest, int count) const
{
	count = getMin(count, mDataLength);
	if (count <= 0)
	{
		return 0;
	}

	const int firstLen = getMin(count, mBufferSize - mHead);
	MEMCPY(dest, count, mBuffer + mHead, firstLen);
	if (firstLen < count)
	{
		MEMCPY((char*)dest + firstLen, count - firstLen, mBuffer, count - firstLen);
	}
	return count;
}

int StreamBuffer::peekData(void* dest, int count, int offset) const
{
	if (offset < 0 || offset >= mDataLength)
	{
		return 0;
	}
	count = getMin(count, mDataLength - offset);
	if (count <= 0)
	{
		return 0;
	}

	// 计算偏移后的起始位置
	const int startPos = (mHead + offset) % mBufferSize;
	const int firstLen = getMin(count, mBufferSize - startPos);
	MEMCPY(dest, count, mBuffer + startPos, firstLen);
	if (firstLen < count)
	{
		MEMCPY((char*)dest + firstLen, count - firstLen, mBuffer, count - firstLen);
	}
	return count;
}

bool StreamBuffer::mergeFrom(const StreamBuffer* other, const bool clearIfFull)
{
	if (other == nullptr || this == other)
	{
		return true;
	}
	const int srcLen = other->mDataLength;
	if (srcLen <= 0)
	{
		return true;
	}

	int freeSpace = mBufferSize - mDataLength;
	if (srcLen > freeSpace)
	{
		if (clearIfFull && mBufferSize >= srcLen)
		{
			clear();
			freeSpace = mBufferSize;
		}
		else
		{
			ERROR("mergeFrom: buffer too small");
			return false;
		}
	}

	// source may wrap around, copy in two segments
	const int srcFirstLen = getMin(srcLen, other->mBufferSize - other->mHead);
	// segment 1: from other.mHead to end of other's buffer
	addDataToBack(other->mBuffer + other->mHead, srcFirstLen);
	// segment 2: wrapped part at the beginning of other's buffer
	if (srcFirstLen < srcLen)
	{
		addDataToBack(other->mBuffer, srcLen - srcFirstLen);
	}
	return true;
}

int StreamBuffer::moveTo(StreamBuffer* target, const int maxCount)
{
	if (target == nullptr || target == this || mDataLength <= 0)
	{
		return 0;
	}

	int moveCount = getMin(mDataLength, target->getFreeSize());
	if (maxCount >= 0)
	{
		moveCount = getMin(moveCount, maxCount);
	}
	if (moveCount <= 0)
	{
		return 0;
	}

	// source可能跨越环形缓冲区末尾,最多分两段写入target。
	// moveCount已经限制在target剩余容量以内,所以addDataToBack不会因为容量不足失败。
	const int firstLen = getMin(moveCount, mBufferSize - mHead);
	if (!target->addDataToBack(mBuffer + mHead, firstLen))
	{
		return 0;
	}
	if (firstLen < moveCount)
	{
		if (!target->addDataToBack(mBuffer, moveCount - firstLen))
		{
			// 理论上不会进入这里,因为前面已经按target剩余容量限制过moveCount。
			// 不回滚第一段,避免引入更复杂的异常路径。
			removeDataFromFront(firstLen);
			return firstLen;
		}
	}
	removeDataFromFront(moveCount);
	return moveCount;
}

void StreamBuffer::linearize()
{
	// Empty or already contiguous: no-op
	if (mDataLength == 0 || getContiguousDataLength() == mDataLength)
	{
		return;
	}

	// Data wraps around: in-place compact to [0, mDataLength)
	const int firstLen = getMin(mDataLength, mBufferSize - mHead);
	const int secondLen = mDataLength - firstLen;

	// Step 1: move the wrapped segment at [0, secondLen) to its final position
	//         destination is always to the right of source → safe with memmove
	if (secondLen > 0)
	{
		MEMMOV(mBuffer + firstLen, mBufferSize - firstLen, mBuffer, secondLen);
	}
	// Step 2: move the first segment from [mHead, ...) to [0, ...)
	//         by this point step1 has already evacuated the area we might overwrite
	if (firstLen > 0)
	{
		MEMMOV(mBuffer, mBufferSize, mBuffer + mHead, firstLen);
	}

	mHead = 0;
	mTail = mDataLength;
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

	// 创建新的缓冲区,将原来的数据拷贝到新缓冲区中(线性化)
	char* newBuffer = ArrayPool::newCharArray(size);
	if (mDataLength > 0)
	{
		const int firstLen = getMin(mDataLength, mBufferSize - mHead);
		MEMCPY(newBuffer, size, mBuffer + mHead, firstLen);
		if (firstLen < mDataLength)
		{
			MEMCPY(newBuffer + firstLen, size - firstLen, mBuffer, mDataLength - firstLen);
		}
	}
	ArrayPool::deleteCharArray(mBuffer, mBufferSize);
	mBufferSize = size;
	mBuffer = newBuffer;
	mHead = 0;
	mTail = mDataLength;
}