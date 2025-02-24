#pragma once

#include "Utility.h"
#include "SerializableData.h"

// 用于二进制数据的读取
class MICRO_LEGEND_FRAME_API SerializerRead
{
public:
	SerializerRead(const char* buffer, int bufferSize);
	template<typename T>
	bool read(T& value)
	{
		return BinaryUtility::read<T>(mBuffer, mBufferSize, mIndex, value);
	}
	bool readVector2(Vector2& vec)
	{
		return BinaryUtility::readVector2(mBuffer, mBufferSize, mIndex, vec);
	}
	bool readVector2Int(Vector2Int& vec)
	{
		return BinaryUtility::readVector2Int(mBuffer, mBufferSize, mIndex, vec);
	}
	bool readVector3(Vector3& vec)
	{
		return BinaryUtility::readVector3(mBuffer, mBufferSize, mIndex, vec);
	}
	bool readVector4(Vector4& vec)
	{
		return BinaryUtility::readVector4(mBuffer, mBufferSize, mIndex, vec);
	}
	// 自定义数据类型,需要继承SerializableData才能通过此方法读取数据
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableData, T>::mType>
	bool readCustom(T& value)
	{
		return value.readFromBuffer(this);
	}
	template<typename T>
	T read()
	{
		return BinaryUtility::read<T>(mBuffer, mBufferSize, mIndex);
	}
	template<typename T>
	bool readList(Vector<T>& list)
	{
		int count;
		if (!read(count))
		{
			return false;
		}
		bool result = true;
		list.resize(count);
		FOR_I(count)
		{
			result = read(list[i]) && result;
		}
		return result;
	}
	bool readStringList(Vector<string>& list);
	// 自定义数据类型,需要继承SerializableData才能通过此方法读取数据
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableData, T>::mType>
	bool readCustomList(Vector<T>& list)
	{
		int count;
		if (!read(count))
		{
			return false;
		}
		bool result = true;
		list.resize(count);
		FOR_I(count)
		{
			result = readCustom(list[i]) && result;
		}
		return result;
	}
	bool readBuffer(char* buffer, int bufferSize, int readLen);
	bool readBufferNoCopy(const char*& destBuffer, int readLength);
	bool readString(char* str, int bufferSize);
	bool readString(string& str);
	void setIndex(const int index)	{ mIndex = index; }
	const char* getBuffer() const	{ return mBuffer; }
	int getBufferSize() const		{ return mBufferSize; }
	int getIndex() const			{ return mIndex; }
protected:
	const char* mBuffer = nullptr;	// 正在读的缓冲区
	int mBufferSize = 0;			// 缓冲区大小
	int mIndex = 0;					// 当前已经读的数据量
};