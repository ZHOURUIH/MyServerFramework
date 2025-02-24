#pragma once

#include "FrameBase.h"
#include "ThreadLock.h"
#include "ThreadLockScope.h"

// 双缓冲,线程安全的缓冲区,可在多个线程中写入数据,另一个线程中读取数据
// 存储的是T类型的列表
template<typename T>
class MICRO_LEGEND_FRAME_API DoubleBuffer
{
public:
	DoubleBuffer():
		mCloseBuffer(false),
		mReading(false)
	{}
	// 尝试检查一下有没有数据可以读,由于没有加锁,所以不保证结果一定正确
	bool tryCheckDataToRead() const { return mBufferList[mWriteIndex].size() > 0; }
	// 切换缓冲区,获得可读列表,在遍历可读列表期间,不能再次调用get,否则会出现不可预知的错误,并且该函数不能在多个线程中调用
	// 可读列表不再访问时需要调用endRead
	// 仅BufferVectorReadScope可调用
	Vector<T>* startRead()
	{
		if (mReading)
		{
			ERROR("当前正在操作读缓冲区,不能再次获取读缓冲区, stack:" + mReadingStack);
			return nullptr;
		}
		{
#ifdef WINDOWS
			if (mEnableStackTrace)
			{
				THREAD_LOCK(mReadingStackLock);
				mReadingStack = getStackTrace(2);
			}
#endif
		}
		mReading = true;

		// 获取读的列表时,不能去添加,可能正在切换读写缓冲区时,另外一个线程正在往写缓冲区内写数据
		THREAD_LOCK(mBufferLock);
		const int curThreadID = getThreadID();
		if (mReadThreadID == 0)
		{
			mReadThreadID = curThreadID;
		}
		if (mReadThreadID != curThreadID)
		{
			ERROR("不能在不同线程中获得可读列表,当前操作线程:" + UIToS(curThreadID) + ",首次操作线程:" + UIToS(mReadThreadID));
			return nullptr;
		}
		MathUtility::swap(mReadIndex, mWriteIndex);
		return &(mBufferList[mReadIndex]);
	}
	// 仅BufferVectorReadScope可调用
	void endRead() { mReading = false; }
	// 适用于基础数据类型
	void add(const Vector<T>& value)
	{
		if (value.size() == 0)
		{
			return;
		}
		if (mCloseBuffer)
		{
			ERROR("缓冲区已经关闭,无法再写入数据");
			return;
		}

		THREAD_LOCK(mBufferLock);
		Vector<T>& writeList = mBufferList[mWriteIndex];
		if (mWriteListLimit == 0 || writeList.size() + value.size() <= mWriteListLimit)
		{
			writeList.addRange(value);
		}
	}
	// 适用于非基础数据类型
	void addCopy(const Vector<T>& value)
	{
		if (value.size() == 0)
		{
			return;
		}
		if (mCloseBuffer)
		{
			ERROR("缓冲区已经关闭,无法再写入数据");
			return;
		}

		THREAD_LOCK(mBufferLock);
		Vector<T>& writeList = mBufferList[mWriteIndex];
		if (mWriteListLimit == 0 || writeList.size() + value.size() <= mWriteListLimit)
		{
			writeList.addRangeCopy(value);
		}
	}
	// 向可写列表中添加数据
	void add(const T& value)
	{
		if (mCloseBuffer)
		{
			ERROR("缓冲区已经关闭,无法再写入数据");
			return;
		}

		THREAD_LOCK(mBufferLock);
		Vector<T>& writeList = mBufferList[mWriteIndex];
		if (mWriteListLimit == 0 || writeList.size() < mWriteListLimit)
		{
			writeList.push_back(value);
		}
	}
	void setWriteListLimit(const int limit) { mWriteListLimit = limit; }
	void setCloseBuffer(const bool close) { mCloseBuffer = close; }
	int getAllDataCount() const { return mBufferList[0].size() + mBufferList[1].size(); }
	static void setEnableStackTrace(bool enable) { mEnableStackTrace = enable; }
	static bool isEnableStackTrace() { return mEnableStackTrace; }
	void clear()
	{
		THREAD_LOCK(mBufferLock);
		mBufferList[mReadIndex].clear();
		mBufferList[mWriteIndex].clear();
		mReadThreadID = 0;
	}
protected:
	Vector<T> mBufferList[2];		// 实际存储数据的缓冲区列表
#ifdef WINDOWS
	ThreadLock mReadingStackLock;	// ReadingStack的锁
#endif
	ThreadLock mBufferLock;			// mBufferList的锁
	string mReadingStack;			// 开始reading的堆栈信息
	int mWriteListLimit = 0;		// 写缓冲区可存储的最大数量,当到达上限时无法再写入,等于0表示无上限
	int mReadThreadID = 0;			// 当前正在读缓冲区的线程ID
	byte mWriteIndex = 0;			// 写缓冲区下标
	byte mReadIndex = 1;			// 读缓冲区下标
	static bool mEnableStackTrace;	// 是否启用堆栈追踪
	atomic<bool> mCloseBuffer;		// 是否已经关闭buffer,关闭后将不能往buffer中写入数据
	atomic<bool> mReading;			// 是否正在操作读缓冲区
};