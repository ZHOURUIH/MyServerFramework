#pragma once

#include "FrameBase.h"

// 环形字节队列缓冲区,数据从尾部添加,从头部移除,无需数据搬移
// 相比线性缓冲区,消除了 addDataToBack 中的 MEMMOV 压缩操作
// 注意: getData() 返回的连续数据长度可能小于 getDataLength()
//       需要处理环形跨越时,请使用 peekData() 或检查 getContiguousDataLength()
class MICRO_LEGEND_FRAME_API StreamBuffer
{
public:
	explicit StreamBuffer(const int bufferSize);
	virtual ~StreamBuffer();
	bool isAvailable(const int count) const { return count <= mBufferSize - mDataLength; }

	// 向尾部添加数据,环形写入,无需搬移已有数据
	bool addDataToBack(const char* data, int count, bool clearIfFull = false);
	bool addEmptyToBack(int count, bool clearIfFull = false);
	// 从头部移除数据,仅移动读指针,零拷贝
	void removeDataFromFront(const int count)
	{
		if (mDataLength < count)
		{
			LOG("删除数据失败!数据量不足");
			return;
		}
		mHead = (mHead + count) % mBufferSize;
		mDataLength -= count;
		// 缓冲区清空时重置指针,避免漂移,最大化后续写入的连续空间
		if (mDataLength == 0)
		{
			mHead = 0;
			mTail = 0;
		}
	}
	void clear()
	{
		mHead = 0;
		mTail = 0;
		mDataLength = 0;
	}
	bool isEmpty() const				{ return mDataLength == 0; }
	int getDataLength() const           { return mDataLength; }
	int getBufferSize() const           { return mBufferSize; }
	int getFreeSize() const             { return mBufferSize - mDataLength; }
	// 获取头部数据指针(第一段连续数据)
	// 注意: 连续数据长度可能小于 getDataLength(), 请用 getContiguousDataLength() 确认
	char* getData() const               { return mBuffer + mHead; }
	int getDataStart() const            { return mHead; }
	// 获取第一段连续数据的长度
	// 数据未跨越缓冲区末尾时等于 getDataLength()
	int getContiguousDataLength() const
	{
		if (mDataLength == 0)
		{
			return 0;
		}
		return getMin(mDataLength, mBufferSize - mHead);
	}
	// 获取尾部可连续写入的空间长度(用于直接 recv 等场景)
	int getContiguousFreeLength() const
	{
		const int freeSpace = mBufferSize - mDataLength;
		if (freeSpace == 0)
		{
			return 0;
		}
		if (mDataLength == 0)
		{
			return mBufferSize;
		}
		if (mTail < mHead)
		{
			return mHead - mTail;
		}
		return mBufferSize - mTail;
	}
	// 将指定长度的数据拷贝到目标缓冲区(自动处理环形跨越)
	int peekData(void* dest, int count) const;
	// 带偏移的 peekData, 从数据头部跳过 offset 字节后读取 count 字节
	int peekData(void* dest, int count, int offset) const;
	// 将 other 的全部数据合并到当前缓冲区尾部, other 不变
	// 空间不足时: clearIfFull=true 先清空再写入, false 则返回失败
	bool mergeFrom(const StreamBuffer* other, bool clearIfFull = false);
	// 将当前缓冲区头部数据移动到target尾部,会从当前缓冲区移除已移动的数据
	// 只移动target当前剩余空间能够容纳的数据,maxCount < 0表示不限制最大移动量
	// 返回实际移动的字节数,空间不足不是错误,只会少移动或返回0
	int moveTo(StreamBuffer* target, int maxCount = -1);
	// 将数据线性化: 全部搬移到 buffer[0] 开始的连续区域
	// 如果数据已经连续(无环形跨越)则不做任何操作
	void linearize();
	// 获取尾部指针(用于直接写入,写后需手动 commitTail)
	char* getTailPtr() const            { return mBuffer + mTail; }
	void commitTail(int count)
	{
		mTail = (mTail + count) % mBufferSize;
		mDataLength += count;
	}
protected:
	void resizeBuffer(int size);
protected:
	char* mBuffer = nullptr;
	int mBufferSize = 0;
	int mHead = 0;          // 读位置
	int mTail = 0;          // 写位置
	int mDataLength = 0;    // 有效数据长度
};