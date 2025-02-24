#pragma once

#include "Utility.h"
#include "SerializableData.h"

// 用于生成二进制文件的
class MICRO_LEGEND_FRAME_API SerializerWrite
{
public:
	void initCapacity(int size);
	virtual ~SerializerWrite();
	template<typename T, typename TypeCheck = typename IsPodType<T>::mType>
	bool write(const T value)
	{
		writeCheck(sizeof(T));
		return BinaryUtility::write(mBuffer, mBufferSize, mIndex, value);
	}
	bool writeVector2(const Vector2& value)
	{
		writeCheck(sizeof(value));
		return BinaryUtility::writeVector2(mBuffer, mBufferSize, mIndex, value);
	}
	bool writeVector2Int(const Vector2Int& value)
	{
		writeCheck(sizeof(value));
		return BinaryUtility::writeVector2Int(mBuffer, mBufferSize, mIndex, value);
	}
	bool writeVector3(const Vector3& value)
	{
		writeCheck(sizeof(value));
		return BinaryUtility::writeVector3(mBuffer, mBufferSize, mIndex, value);
	}
	bool writeVector4(const Vector4& value)
	{
		writeCheck(sizeof(value));
		return BinaryUtility::writeVector4(mBuffer, mBufferSize, mIndex, value);
	}
	// 自定义数据类型可以使用此接口,但是需要继承SerializableData
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableData, T>::mType>
	bool writeCustom(const T& value)
	{
		return value.writeToBuffer(this);
	}
	template<typename T, typename TypeCheck = typename IsPodType<T>::mType>
	bool writeList(const Vector<T>& list)
	{
		const int count = list.size();
		if (!write(count))
		{
			return false;
		}
		bool result = true;
		FOR_I(count)
		{
			result = write(list[i]) && result;
		}
		return result;
	}
	bool writeStringList(const Vector<string>& list);
	// 自定义数据类型可以使用此接口,但是需要继承SerializableData
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableData, T>::mType>
	bool writeCustomList(const Vector<T>& list)
	{
		const int count = list.size();
		if (!write(count))
		{
			return false;
		}
		bool result = true;
		FOR_I(count)
		{
			result = writeCustom(list[i]) && result;
		}
		return result;
	}
	bool writeBuffer(const char* buffer, int bufferSize);
	bool writeString(const char* str);
	bool writeString(const string& str);
	bool writeToFile(const string& fullName) const;
	void setIndex(const int index)	{ mIndex = index; }
	const char* getBuffer() const	{ return mBuffer; }
	int getBufferSize() const		{ return mBufferSize; }
	int getDataSize() const			{ return mIndex; }
	void clear()					{ mIndex = 0; }
protected:
	void writeCheck(int writeLen);
protected:
	char* mBuffer = nullptr;	// 正在写的缓冲区
	int mBufferSize = 0;		// 当前缓冲区大小,始终都是2的n次方
	int mIndex = 0;				// 当前已写入的数据量
};