#pragma once

#include "Utility.h"
#include "SerializableBitData.h"

// 用于生成二进制文件的
class MICRO_LEGEND_FRAME_API SerializerBitWrite
{
public:
	SerializerBitWrite(bool onlyMain = true):
		mMainThreadOnly(onlyMain){}
	void initCapacity(int size);
	virtual ~SerializerBitWrite();
	bool writeBool(const bool value)
	{
		writeCheck(sizeof(bool));
		return writeBoolBit(mBuffer, mBufferSize, mBitIndex, value);
	}
	// 可传入两个及以上个T类型的参数
	template<typename T, typename... TypeList>
	bool writeSigned(T value0, T value1, TypeList... params)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<T, TypeList>...>::value, "All types must be the same");
#endif
		static_assert(isSignedInteger<T>(), "All types must be POD signed integer types");
		constexpr int size = sizeof...(params) + 2;
		return writeSigned<size, T>(Array<size, T>{ value0, value1, forward<TypeList>(params)... });
	}
	template<int Count, typename T>
	bool writeSigned(const Array<Count, T>& list)
	{
		writeCheck(sizeof(int) + 1 + sizeof(T) * Count);
		return writeSignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list);
	}
	template<typename T>
	bool writeSigned(const T value)
	{
		// 因为最差情况下实际所需的空间比value自身占的空间多8bit,7bit长度位+1bit符号位,所以需要多扩容1个字节
		writeCheck(sizeof(value) + 1);
		return writeSignedIntegerBit(mBuffer, mBufferSize, mBitIndex, value);
	}
	// 可传入两个及以上个T类型的参数
	template<typename T, typename... TypeList>
	bool writeUnsigned(const T value0, const T value1, TypeList... params)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<T, TypeList>...>::value, "All types must be the same");
#endif
		static_assert(isUnsignedInteger<T>(), "All types must be POD unsigned integer types");
		constexpr int size = sizeof...(params) + 2;
		return writeUnsigned<size, T>(Array<size, T>{ value0, value1, forward<TypeList>(params)... });
	}
	template<int Count, typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsigned(const Array<Count, T>& list)
	{
		writeCheck(sizeof(int) + 1 + sizeof(T) * Count);
		return writeUnsignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list);
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsigned(const T value)
	{
		// 因为最差情况下实际所需的空间比value自身占的空间多7bit长度位,所以需要多扩容1个字节
		writeCheck(sizeof(value) + 1);
		return writeUnsignedIntegerBit(mBuffer, mBufferSize, mBitIndex, value);
	}
	bool writeFloat(const float value, const int precision = 3)
	{
		return writeSigned(MathUtility::round(value * pow10(precision)));
	}
	// 可传入两个及以上个T类型的参数
	template<typename... TypeList>
	bool writeFloat(float value0, float value1, TypeList... params)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<float, TypeList>...>::value, "All types must be the same");
#endif
		return writeFloat(Array<sizeof...(params) + 2, float>{ value0, value1, forward<TypeList>(params)... });
	}
	template<int Count>
	bool writeFloat(const Array<Count, float>& list)
	{
		writeCheck(sizeof(int) + 1 + sizeof(float) * Count);
		return writeFloatListBit(mBuffer, mBufferSize, mBitIndex, list, 3);
	}
	bool writeDouble(const double value, const int precision = 4) { return writeSigned(roundDouble(value * pow10LLong(precision))); }
	// 可传入两个及以上个T类型的参数
	template<typename... TypeList>
	bool writeDouble(const double value0, const double value1, TypeList... params)
	{
#ifdef WINDOWS
		static_assert(conjunction<is_same<double, TypeList>...>::value, "All types must be the same");
#endif
		return writeDouble(Array<sizeof...(params) + 2, double>{ value0, value1, forward<TypeList>(params)... });
	}
	template<int Count>
	bool writeDouble(const Array<Count, double>& list)
	{
		writeCheck(sizeof(int) + 1 + sizeof(double) * Count);
		return writeDouble(mBuffer, mBufferSize, mBitIndex, list, 4);
	}
	bool writeVector2(const Vector2& value) { return writeFloat(value.x, value.y); }
	bool writeVector2Int(const Vector2Int& value) { return writeSigned(value.x, value.y); }
	bool writeVector2UInt(const Vector2UInt& value) { return writeUnsigned(value.x, value.y); }
	bool writeVector2Short(const Vector2Short& value) { return writeSigned(value.x, value.y); }
	bool writeVector2UShort(const Vector2UShort& value) { return writeUnsigned(value.x, value.y); }
	bool writeVector3(const Vector3& value) { return writeFloat(value.x, value.y, value.z); }
	bool writeVector4(const Vector4& value) { return writeFloat(value.x, value.y, value.z, value.w); }
	// 自定义数据类型可以使用此接口,但是需要继承SerializableData
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableBitData, T>::mType>
	bool writeCustom(const T& value) { return value.writeToBuffer(this); }
	// 将最后一个字节未填充数据的位填充为0,并将位下标移动到字节末尾
	void fillZeroToByteEnd() { BinaryUtility::fillZeroToByteEnd(mBuffer, mBitIndex); }
	bool writeBuffer(const char* buffer, int dataCount)
	{
		writeCheck(dataCount);
		return writeBufferBit(mBuffer, mBufferSize, mBitIndex, buffer, dataCount);
	}
	bool writeString(const string& str)
	{
		// 先写入字符串长度
		const int writeLen = (int)str.length();
		if (!writeSigned(writeLen))
		{
			return false;
		}
		return writeBuffer(str.c_str(), writeLen);
	}
	template<typename T>
	bool writeSignedList(const Vector<T>& list)
	{
		writeCheck(sizeof(int) + 1 + sizeof(T) * list.size());
		return writeSignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list);
	}
	template<typename T>
	bool writeUnsignedList(const Vector<T>& list)
	{
		writeCheck(sizeof(int) + 1 + sizeof(T) * list.size());
		return writeUnsignedIntegerListBit(mBuffer, mBufferSize, mBitIndex, list);
	}
	bool writeFloatList(const Vector<float>& list, const int precision = 3)
	{
		writeCheck(sizeof(int) + 1 + sizeof(float) * list.size());
		return writeFloatListBit(mBuffer, mBufferSize, mBitIndex, list, precision);
	}
	bool writeDoubleList(const Vector<double>& list, const int precision = 4)
	{
		writeCheck(sizeof(int) + 1 + sizeof(double) * list.size());
		return writeDoubleListBit(mBuffer, mBufferSize, mBitIndex, list, precision);
	}
	// 自定义数据类型可以使用此接口,但是需要继承SerializableData
	template<typename T, typename TypeCheck = typename IsSubClassOf<SerializableBitData, T>::mType>
	bool writeCustomList(const Vector<T>& list)
	{
		const int count = list.size();
		if (!writeSigned(count))
		{
			return false;
		}
		bool result = true;
		FOR_I(count)
		{
			result = result && writeCustom(list[i]);
		}
		return result;
	}
	bool writeStringList(const Vector<string>& list);
	const char* getBuffer() const			{ return mBuffer; }
	int getBufferSize() const				{ return mBufferSize; }
	int getBitCount() const					{ return mBitIndex; }
	int getByteCount() const				{ return bitCountToByteCount(mBitIndex); }
	void clear()
	{
		if (mBuffer != nullptr)
		{
			memset(mBuffer, 0, getByteCount());
		}
		mBitIndex = 0;
	}
protected:
	void writeCheck(int writeLen);
	char* newChar(int size) const;
	void deleteChar(char*& buffer, int size) const;
protected:
	char* mBuffer = nullptr;	// 正在写的缓冲区
	int mBufferSize = 0;		// 当前缓冲区大小,始终都是2的n次方
	int mBitIndex = 0;			// 当前已写入的bit数量,需要 >> 3转换为字节数量
	bool mMainThreadOnly = true;// 是否只允许在主线程中使用,如果在子线程中使用,则会使用线程安全的对象池申请内存,但是效率会低一些,但是不能跨线程调用
};