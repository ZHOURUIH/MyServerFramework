#pragma once

#include "Utility.h"
#include "SerializableBitData.h"

// 用于生成二进制文件的
class MICRO_LEGEND_FRAME_API SerializerBitRead
{
public:
	SerializerBitRead(const char* buffer, int bufferSize);
	bool readBool(bool& value) { return readBoolBit(mBuffer, mBufferSize, mBitIndex, value); }
	bool readBuffer(char* destBuffer, const int readLength)
	{
		return destBuffer != nullptr && readBufferBit(mBuffer, mBufferSize, mBitIndex, destBuffer, readLength, readLength);
	}
	bool readBufferNoCopy(const char*& destBuffer, int readLength);
	// 可以读取2个及以上的参数
	template<typename T, typename... Args>
	bool readSigned(T& value0, T& value1, Args&... args)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<T, Args>...>::value, "All types must be the same");
#endif
		static_assert(isSignedInteger<T>(), "All types must be POD signed integer types");
		Array<sizeof...(args) + 2, T*> list{ &value0, & value1, & args... };
		return readSignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list);
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSigned(T& value) { return readSignedIntegerBit(mBuffer, mBufferSize, mBitIndex, value); }
	// 可以读取2个及以上的参数
	template<typename T, typename... Args>
	bool readUnsigned(T& value0, T& value1, Args&... args)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<T, Args>...>::value, "All types must be the same");
#endif
		static_assert(isUnsignedInteger<T>(), "All types must be POD unsigned integer types");
		Array<sizeof...(args) + 2, T*> list{ &value0, & value1, & args... };
		return readUnsignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list);
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsigned(T& value) { return readUnsignedIntegerBit(mBuffer, mBufferSize, mBitIndex, value); }
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedList(Vector<T>& list) { return readSignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list); }
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedList(Vector<T>& list) { return readUnsignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list); }
	// 可以读取2个及以上的参数
	template<typename... Args>
	bool readFloat(float& value0, float& value1, Args&... args)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<float, Args>...>::value, "All types must be the same");
#endif
		Array<sizeof...(args) + 2, float*> list{ &value0, & value1, & args... };
		return readFloatListBit(mBuffer, mBufferSize, mBitIndex, list, 3);
	}
	bool readFloat(float& value, const int precision = 3)
	{
		return readFloatBit(mBuffer, mBufferSize, mBitIndex, value, precision);
	}
	// 可以读取2个及以上的参数
	template<typename... Args>
	bool readDouble(double& value0, double& value1, Args&... args)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<double, Args>...>::value, "All types must be the same");
#endif
		Array<sizeof...(args) + 2, double*> list{ &value0, & value1, & args... };
		return readDoubleListBit(mBuffer, mBufferSize, mBitIndex, list, 4);
	}
	bool readDouble(double& value, const int precision = 4) { return readDoubleBit(mBuffer, mBufferSize, mBitIndex, value, precision); }
	bool readVector2(Vector2& vec) { return readFloat(vec.x, vec.y); }
	bool readVector2Int(Vector2Int& vec) { return readSigned(vec.x, vec.y); }
	bool readVector2UInt(Vector2UInt& vec) { return readUnsigned(vec.x, vec.y); }
	bool readVector2Short(Vector2Short& vec) { return readSigned(vec.x, vec.y); }
	bool readVector2UShort(Vector2UShort& vec) { return readUnsigned(vec.x, vec.y); }
	bool readVector3(Vector3& vec) { return readFloat(vec.x, vec.y, vec.z); }
	bool readVector4(Vector4& vec) { return readFloat(vec.x, vec.y, vec.z, vec.w); }
	// 自定义数据类型,需要继承SerializableData才能通过此方法读取数据
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableBitData, T>::mType>
	bool readCustom(T& value) { return value.readFromBuffer(this); }
	bool readString(string& str);
	// 自定义数据类型,需要继承SerializableData才能通过此方法读取数据
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableBitData, T>::mType>
	bool readCustomList(Vector<T>& list)
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
			result = result && readCustom(list[i]);
		}
		return result;
	}
	bool readFloatList(Vector<float>& list, const int precision = 3) { return readFloatListBit(mBuffer, mBufferSize, mBitIndex, list, precision); }
	bool readDoubleList(Vector<double>& list, const int precision = 4){ return readDoubleListBit(mBuffer, mBufferSize, mBitIndex, list, precision); }
	bool readStringList(Vector<string>& list);
	void skipToByteEnd()			{ mBitIndex = bitCountToByteCount(mBitIndex) << 3; }
	void setBitIndex(int index)		{ mBitIndex = index; }
	const char* getBuffer() const	{ return mBuffer; }
	int getBufferSize() const		{ return mBufferSize; }
	int getBitIndex() const			{ return mBitIndex; }
	int getReadByteCount() const	{ return bitCountToByteCount(mBitIndex); }
protected:
	const char* mBuffer = nullptr;	// 正在读的缓冲区
	int mBufferSize = 0;			// 缓冲区大小
	int mBitIndex = 0;				// 当前已读取的bit数量,需要 >> 3转换为字节数量
};