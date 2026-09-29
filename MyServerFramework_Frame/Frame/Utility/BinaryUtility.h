#pragma once

#include "TypeUtility.h"
#include "ThreadLock.h"

namespace BinaryUtility
{
	// constexpr的定义只能写在头文件中,否则在其他地方使用时会提示计算结果不是常量,无法使定义结果为constexpr
	// 不过将定义写到头文件中带来的影响就是每个cpp文件中的常量地址是不一样的

	// 下标是sizeof(T),value是此T类型的bit长度所需要的长度表示
	// 比如1个char的取值范围是-127~127,去除符号以后是0~127,127占7个bit,可以使用3个bit存储这个7,所以长度位就是用3个bit来表示,所以下标1的值是3
	constexpr byte SIGNED_LENGTH_MAX_BIT[9]{0, 3, 4, 0, 5, 0, 0, 0, 6};
	// 下标是sizeof(T),value是此T类型的bit长度所需要的长度表示
	// 比如1个byte的取值范围是0~255,255占8个bit,原本8的二进制是4个bit(1000),但是大多数情况下几乎不会超过127,所以大多数时候最多到7,也就是(111)即可,所以只用3个bit来表示,所以下标1(也就是1byte)对应的值是3(表示长度位固定为3bit)
	// 但是也不能不考虑超过127的情况,所以当长度位是7(111)时,就直接读取8个bit的数据位
	// 其作用于mBitCountTable类似,只不过不需要计算
	constexpr byte UNSIGNED_LENGTH_MAX_BIT[9]{ 0, 3, 4, 0, 5, 0, 0, 0, 6 };
	// 0到65535的每个数中的最高位1的下标,也就是需要用几个bit来表示,1可以用1个bit表示,5可以使用3个bit表示,8可以使用5个bit表示
	MICRO_LEGEND_FRAME_API extern byte mBitCountTable[65536];
	MICRO_LEGEND_FRAME_API extern ThreadLock mBitCountTableLock;
	// uint完整按位编码缓存。0~2^22-1的最终编码最多27bit,低27bit保存完整编码,高5bit保存总bit数
	constexpr uint UINT_WRITE_CACHE_COUNT_64K = 1u << 16;
	constexpr uint UINT_WRITE_CACHE_COUNT_256K = 1u << 18;
	constexpr uint UINT_WRITE_CACHE_COUNT_1M = 1u << 20;
	constexpr uint UINT_WRITE_CACHE_COUNT_4M = 1u << 22;
	constexpr uint UINT_WRITE_CACHE_PRODUCTION_COUNT = UINT_WRITE_CACHE_COUNT_256K;
	constexpr uint USHORT_WRITE_CACHE_COUNT = 1u << 16;
	constexpr byte UINT_WRITE_CACHE_BIT_COUNT_SHIFT = 27;
	constexpr uint UINT_WRITE_CACHE_BITS_MASK = (1u << UINT_WRITE_CACHE_BIT_COUNT_SHIFT) - 1u;
	MICRO_LEGEND_FRAME_API extern uint mUIntWriteCache[UINT_WRITE_CACHE_PRODUCTION_COUNT];
	MICRO_LEGEND_FRAME_API extern uint mUShortWriteCache[USHORT_WRITE_CACHE_COUNT];
	MICRO_LEGEND_FRAME_API extern const ushort crc16_table[256];
	// 各个基础数据类型的类型hash值
	MICRO_LEGEND_FRAME_API extern const int mStringType;
	MICRO_LEGEND_FRAME_API extern const int mBoolType;
	MICRO_LEGEND_FRAME_API extern const int mCharType;
	MICRO_LEGEND_FRAME_API extern const int mWCharType;
	MICRO_LEGEND_FRAME_API extern const int mByteType;
	MICRO_LEGEND_FRAME_API extern const int mShortType;
	MICRO_LEGEND_FRAME_API extern const int mUShortType;
	MICRO_LEGEND_FRAME_API extern const int mIntType;
	MICRO_LEGEND_FRAME_API extern const int mUIntType;
	MICRO_LEGEND_FRAME_API extern const int mFloatType;
	MICRO_LEGEND_FRAME_API extern const int mLLongType;
	MICRO_LEGEND_FRAME_API extern const int mULLongType;
	MICRO_LEGEND_FRAME_API extern const int mBoolPtrType;
	MICRO_LEGEND_FRAME_API extern const int mCharPtrType;
	MICRO_LEGEND_FRAME_API extern const int mBytePtrType;
	MICRO_LEGEND_FRAME_API extern const int mShortPtrType;
	MICRO_LEGEND_FRAME_API extern const int mUShortPtrType;
	MICRO_LEGEND_FRAME_API extern const int mIntPtrType;
	MICRO_LEGEND_FRAME_API extern const int mUIntPtrType;
	MICRO_LEGEND_FRAME_API extern const int mFloatPtrType;
	MICRO_LEGEND_FRAME_API extern const int mLLongPtrType;
	MICRO_LEGEND_FRAME_API extern const int mULLongPtrType;
	MICRO_LEGEND_FRAME_API extern const int mBoolListType;
	MICRO_LEGEND_FRAME_API extern const int mCharListType;
	MICRO_LEGEND_FRAME_API extern const int mByteListType;
	MICRO_LEGEND_FRAME_API extern const int mShortListType;
	MICRO_LEGEND_FRAME_API extern const int mUShortListType;
	MICRO_LEGEND_FRAME_API extern const int mIntListType;
	MICRO_LEGEND_FRAME_API extern const int mUIntListType;
	MICRO_LEGEND_FRAME_API extern const int mFloatListType;
	MICRO_LEGEND_FRAME_API extern const int mLLongListType;
	MICRO_LEGEND_FRAME_API extern const int mULLongListType;
	MICRO_LEGEND_FRAME_API extern const int mVector2Type;
	MICRO_LEGEND_FRAME_API extern const int mVector2IntType;
	MICRO_LEGEND_FRAME_API extern const int mVector2UIntType;
	MICRO_LEGEND_FRAME_API extern const int mVector2ShortType;
	MICRO_LEGEND_FRAME_API extern const int mVector2UShortType;
	MICRO_LEGEND_FRAME_API extern const int mVector3Type;
	MICRO_LEGEND_FRAME_API extern const int mVector3IntType;
	MICRO_LEGEND_FRAME_API extern const int mVector2ListType;
	MICRO_LEGEND_FRAME_API extern const int mVector2IntListType;
	MICRO_LEGEND_FRAME_API extern const int mVector3ListType;
	MICRO_LEGEND_FRAME_API extern const int mVector3IntListType;

	//------------------------------------------------------------------------------------------------------------------------------
	// protected
	MICRO_LEGEND_FRAME_API void initBitCountTable();
	MICRO_LEGEND_FRAME_API void initUIntWriteCache();
	MICRO_LEGEND_FRAME_API void initUShortWriteCache();
	MICRO_LEGEND_FRAME_API byte internalGenerateBitCount(ushort value);
	//------------------------------------------------------------------------------------------------------------------------------
	// public
	// 因为当前类中需要使用过,如果写到FrameUtility中,会有头文件互相包含的问题,所以即使下面的函数于二进制操作无关,也写到这里
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	T getListMax(const Vector<T>& list)
	{
		const int count = list.size();
		if (count == 0)
		{
			return 0;
		}

		T maxValue = list[0];
		for (int i = 1; i < count; ++i)
		{
			if (maxValue < list[i])
			{
				maxValue = list[i];
			}
		}
		return maxValue;
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	T getListMax(const T* list, const int count)
	{
		if (count == 0)
		{
			return 0;
		}

		T maxValue = list[0];
		for (int i = 1; i < count; ++i)
		{
			if (maxValue < list[i])
			{
				maxValue = list[i];
			}
		}
		return maxValue;
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	T getListMaxAbs(const Vector<T>& list)
	{
		const int count = list.size();
		if (count == 0)
		{
			return 0;
		}

		T maxAbsValue = list[0] > 0 ? list[0] : -list[0];
		for (int i = 1; i < count; ++i)
		{
			T absValue = list[i] > 0 ? list[i] : -list[i];
			if (maxAbsValue < absValue)
			{
				maxAbsValue = absValue;
			}
		}
		return maxAbsValue;
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	T getListMaxAbs(const T* list, const int count)
	{
		if (count == 0)
		{
			return 0;
		}

		T maxAbsValue = list[0] > 0 ? list[0] : -list[0];
		for (int i = 1; i < count; ++i)
		{
			T absValue = list[i] > 0 ? list[i] : -list[i];
			if (maxAbsValue < absValue)
			{
				maxAbsValue = absValue;
			}
		}
		return maxAbsValue;
	}
	MICRO_LEGEND_FRAME_API float getFloatListMaxAbs(const float* list, int count);
	MICRO_LEGEND_FRAME_API double getDoubleListMaxAbs(const double* list, int count);
	// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setByte(T& value, const byte b, const int pos) { value = (value & ~(0xFFLL << (8 * pos))) | ((T)b << (8 * pos)); }
	// 获得value的指定位置pos的字节的值
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr byte getByte(T value, const int pos) { return ((value) & (0xFFLL << (8 * pos))) >> (8 * pos); }
	// 指定下标的位是否为1
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr bool hasBit(T value, const int pos) { return (value & ((T)1 << pos)) != 0; }
	// 获取指定位的值，返回 0 或 1
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr int getBit(T value, const int pos) { return (int)(((typename std::make_unsigned<T>::type)value >> pos) & 1); }
	// 设置指定位的值
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setBit(T& value, const int pos, const int bit)
	{
		if (bit != 0)
		{
			value |= (T)1 << pos;
		}
		else
		{
			value &= ~((T)1 << pos);
		}
	}
	// 注意,负数由于存储是使用补码存储的,所以不能直接按位或
	// 将指定下标的位设置为1
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setBitOne(T& value, const int pos) { value |= (T)1 << pos; }
	// 将指定下标的位设置为0
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setBitZero(T& value, const int pos) { value &= ~((T)1 << pos); }
	// 获取最高位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr int getHighestBit(T value) { return (int)(((typename std::make_unsigned<T>::type)value) >> (sizeof(T) * 8 - 1)); }
	// 设置最高位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setHighestBit(T& value, const bool bit)
	{
		if (bit)
		{
			value |= (T)1 << (sizeof(T) * 8 - 1);
		}
		else
		{
			value &= ~((T)1 << (sizeof(T) * 8 - 1));
		}
	}
	// 获取最低位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr int getLowestBit(T value) { return value & (T)1; }
	// 设置最低位
	template<typename T, typename TypeCheck = typename IsPodIntegerType<T>::mType>
	constexpr void setLowestBit(T& value, const bool bit)
	{
		if (bit)
		{
			value |= (T)1;
		}
		else
		{
			value &= ~(T)1;
		}
	}
	// 将value中截取从start开始的count个位的值
	constexpr byte getBits(const byte value, const byte start, const byte count) { return (value & (((1 << count) - 1) << start)) >> start; }
	// 将位的数量转换为字节的数量,比如9个bit就是占2个字节
	constexpr int bitCountToByteCount(const int bitCount) { return (bitCount & 7) != 0 ? (bitCount >> 3) + 1 : bitCount >> 3; }
	inline void setBufferBitOne(char* buffer, const int bitIndex) { setBitOne(buffer[bitIndex >> 3], bitIndex & 7); }
	inline bool getBufferBit(const char* buffer, const int bitIndex) { return hasBit(buffer[bitIndex >> 3], bitIndex & 7); }
	// 将sourceValue中sourceStart开始的writeBitCount个位写入到destValue从destStart开始的位下标上
	inline void copyBits(byte& destValue, const byte destStart, const byte sourceValue, const byte sourceStart, const byte writeBitCount)
	{
		destValue |= (getBits(sourceValue, sourceStart, writeBitCount) << destStart);
	}
	inline void copyBits(char& destValue, const byte destStart, const byte sourceValue, const byte sourceStart, const byte writeBitCount)
	{
		destValue |= (getBits(sourceValue, sourceStart, writeBitCount) << destStart);
	}
	constexpr void copyBits(byte& destValue, const byte destStart, const byte sourceValue) { destValue |= (sourceValue << destStart); }
	constexpr void copyBits(char& destValue, const byte destStart, const byte sourceValue) { destValue |= (sourceValue << destStart); }
	void logErrorInternal(string&& info);
	// 计算有效位数,也就是最高位1的下标+1,比如输入8,返回值就是4,意义是可以使用4个bit就可以表示数字8
	inline byte generateBitCount(const byte value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		return mBitCountTable[value];
	}
	inline byte generateBitCount(const ushort value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		return mBitCountTable[value];
	}
	inline byte generateBitCount(const uint value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		const ushort part1 = (ushort)((value & 0xFFFF0000) >> 16);
		return part1 > 0 ? mBitCountTable[part1] + 16 : mBitCountTable[value & 0x0000FFFF];
	}
	inline byte generateBitCount(const ullong value)
	{
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		if ((value & 0xFFFFFFFF00000000) > 0)
		{
			const ushort part3 = (ushort)((value & 0xFFFF000000000000) >> 48);
			if (part3 > 0)
			{
				return mBitCountTable[part3] + 16 * 3;
			}
			return mBitCountTable[(ushort)((value & 0x0000FFFF00000000) >> 32)] + 16 * 2;
		}
		else
		{
			const ushort part1 = (ushort)((value & 0x00000000FFFF0000) >> 16);
			return part1 > 0 ? mBitCountTable[part1] + 16 * 1 : mBitCountTable[value & 0x000000000000FFFF];
		}
	}
	// 生成uint的完整按位编码缓存项,低27bit是最终编码,高5bit是总bit数。仅用于0~2^22-1
	inline uint encodeUIntWriteCacheValue(const uint value)
	{
		const byte bitCount = generateBitCount(value);
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(uint)];
		constexpr byte REMOVE_HIGH_BIT_LIMIT = (1 << sizeof(uint)) - 1;
		const bool removeHighBit = bitCount > 0 && bitCount < REMOVE_HIGH_BIT_LIMIT;
		const byte dataBitCount = bitCount == 0 ? 0 : (removeHighBit ? bitCount - 1 : bitCount);
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
		const uint dataMask = dataBitCount == 0 ? 0u : (1u << dataBitCount) - 1u;
		const uint packedValue = (uint)bitCount | ((value & dataMask) << TYPE_LENGTH_MAX_BIT);
		return packedValue | ((uint)totalBitCount << UINT_WRITE_CACHE_BIT_COUNT_SHIFT);
	}
	// 生成ushort的完整按位编码缓存项。ushort最终编码最多20bit,加上任意bitOffset后仍可使用32位窗口一次写入。
	inline uint encodeUShortWriteCacheValue(const ushort value)
	{
		const byte bitCount = generateBitCount(value);
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(ushort)];
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		constexpr byte REMOVE_HIGH_BIT_LIMIT = (1 << sizeof(ushort)) - 1;
		byte writeLength = bitCount;
		byte dataLength = bitCount;
		if (bitCount == 1 << TYPE_LENGTH_MAX_BIT)
		{
			writeLength = bitCount - 1;
		}
		else if (bitCount == LENGTH_MAX_VALUE)
		{
			++dataLength;
		}
		const bool removeHighBit = value != 0 && dataLength < REMOVE_HIGH_BIT_LIMIT;
		const byte dataBitCount = value == 0 ? 0 : (removeHighBit ? dataLength - 1 : dataLength);
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
		const uint dataMask = dataBitCount == 0 ? 0u : (1u << dataBitCount) - 1u;
		const uint packedValue = (uint)writeLength | (((uint)value & dataMask) << TYPE_LENGTH_MAX_BIT);
		return packedValue | ((uint)totalBitCount << UINT_WRITE_CACHE_BIT_COUNT_SHIFT);
	}
	// 从buffer的bitIndex的位下标处读取writeBitCount个位,位数最多不超过8位
	inline void readByteBits(const char* buffer, int& bitIndex, byte& value, const byte readBitCount)
	{
		value = ((*((short*)(buffer + (bitIndex >> 3)))) >> (bitIndex & 7)) & ((1 << readBitCount) - 1);
		bitIndex += readBitCount;
	}
	// 读取长度位
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedLengthBit(const char* buffer, const int bufferSize, int& bitIndex, byte& bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		readByteBits(buffer, bitIndex, bitCount, TYPE_LENGTH_MAX_BIT);
		// 因为长度位的存储会忽略一个bit,所以当长度位的bit数达到此类型的最大时,会加1
		// 比如byte类型为8bit,长度位存储只用了3bit,只能表示0到7,所以当读取到的长度位值为7时,就需要读8bit的数据位,不然就会丢失数据
		if (bitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
		{
			++bitCount;
		}
		return true;
	}
	// 读取长度位
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedLengthBit(const char* buffer, const int bufferSize, int& bitIndex, byte& bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		readByteBits(buffer, bitIndex, bitCount, TYPE_LENGTH_MAX_BIT);
		return true;
	}
	// 将一个字节的前writeBitCount个位写入到buffer的bitIndex的位下标处
	inline void writeByteBits(char* buffer, int& bitIndex, const byte value, const byte writeBitCount)
	{
		*((short*)(buffer + (bitIndex >> 3))) |= ((short)value & ((1 << writeBitCount) - 1)) << (bitIndex & 7);
		bitIndex += writeBitCount;
	}
	// 读取不足8字节的尾部数据,避免运行时长度memcpy在短消息尾部产生额外开销
	inline ullong readIntegerTail(const char* buffer, const int byteCount)
	{
		switch (byteCount)
		{
		case 7: return (ullong)*((const uint*)buffer) | ((ullong)*((const ushort*)(buffer + 4)) << 32) | ((ullong)*((const byte*)(buffer + 6)) << 48);
		case 6: return (ullong)*((const uint*)buffer) | ((ullong)*((const ushort*)(buffer + 4)) << 32);
		case 5: return (ullong)*((const uint*)buffer) | ((ullong)*((const byte*)(buffer + 4)) << 32);
		case 4: return (ullong)*((const uint*)buffer);
		case 3: return (ullong)*((const ushort*)buffer) | ((ullong)*((const byte*)(buffer + 2)) << 16);
		case 2: return (ullong)*((const ushort*)buffer);
		case 1: return (ullong)*((const byte*)buffer);
		default: return 0;
		}
	}
	// 一次读取最多64位整数数据,用于整数按位序列化的快速路径
	inline ullong readIntegerBits(const char* buffer, const int bufferSize, int& bitIndex, const byte readBitCount)
	{
		if (readBitCount == 0)
		{
			return 0;
		}
		const int byteIndex = bitIndex >> 3;
		const byte bitOffset = (byte)(bitIndex & 7);
		const int remainByteCount = bufferSize - byteIndex;
		ullong source = 0;
		if (remainByteCount >= 8)
		{
			source = *((const ullong*)(buffer + byteIndex));
		}
		else if (remainByteCount > 0)
		{
			source = readIntegerTail(buffer + byteIndex, remainByteCount);
		}
		ullong value = source >> bitOffset;
		if (bitOffset != 0 && readBitCount > 64 - bitOffset)
		{
			value |= (ullong)(byte)buffer[byteIndex + 8] << (64 - bitOffset);
		}
		if (readBitCount < 64)
		{
			value &= (1ULL << readBitCount) - 1ULL;
		}
		bitIndex += readBitCount;
		return value;
	}
	// 一次写入最多64位整数数据,用于整数按位序列化的快速路径
	inline void writeIntegerBits(char* buffer, const int bufferSize, int& bitIndex, ullong value, const byte writeBitCount)
	{
		if (writeBitCount == 0)
		{
			return;
		}
		if (writeBitCount < 64)
		{
			value &= (1ULL << writeBitCount) - 1ULL;
		}
		const int byteIndex = bitIndex >> 3;
		const byte bitOffset = (byte)(bitIndex & 7);
		const int remainByteCount = bufferSize - byteIndex;
		const byte writeByteCount = (byte)bitCountToByteCount(bitOffset + writeBitCount);
		const ullong writeValue = value << bitOffset;
		if (remainByteCount >= 8)
		{
			*((ullong*)(buffer + byteIndex)) |= writeValue;
		}
		else
		{
			ullong dest = 0;
			memcpy(&dest, buffer + byteIndex, writeByteCount);
			dest |= writeValue;
			memcpy(buffer + byteIndex, &dest, writeByteCount);
		}
		if (bitOffset != 0 && writeBitCount > 64 - bitOffset)
		{
			buffer[byteIndex + 8] |= (char)(value >> (64 - bitOffset));
		}
		bitIndex += writeBitCount;
	}
	// 写入一个已经预编码完成的uint缓存项,用于生产缓存和不同缓存范围的基准测试
	inline bool writeEncodedUIntCacheBit(char* buffer, const int bufferSize, int& bitIndex, const uint cacheValue)
	{
		const byte writeBitCount = (byte)(cacheValue >> UINT_WRITE_CACHE_BIT_COUNT_SHIFT);
		const int endBitIndex = bitIndex + writeBitCount;
		if (bitCountToByteCount(endBitIndex) > bufferSize)
		{
			return false;
		}
		const uint bits = cacheValue & UINT_WRITE_CACHE_BITS_MASK;
		const int byteIndex = bitIndex >> 3;
		const byte bitOffset = (byte)(bitIndex & 7);
		const int remainByteCount = bufferSize - byteIndex;
		const byte writeByteCount = (byte)bitCountToByteCount(bitOffset + writeBitCount);
		const ullong writeValue = (ullong)bits << bitOffset;
		if (remainByteCount >= 8)
		{
			*((ullong*)(buffer + byteIndex)) |= writeValue;
		}
		else
		{
			ullong dest = 0;
			memcpy(&dest, buffer + byteIndex, writeByteCount);
			dest |= writeValue;
			memcpy(buffer + byteIndex, &dest, writeByteCount);
		}
		bitIndex = endBitIndex;
		return true;
	}
	// 生产uint缓存只覆盖0~2^18-1,最终编码最多23bit,再加bitOffset最多30bit,固定可以用32位窗口完成写入
	inline bool writeEncodedUIntCache32Bit(char* buffer, const int bufferSize, int& bitIndex, const uint cacheValue)
	{
		const byte writeBitCount = (byte)(cacheValue >> UINT_WRITE_CACHE_BIT_COUNT_SHIFT);
		const int endBitIndex = bitIndex + writeBitCount;
		if (bitCountToByteCount(endBitIndex) > bufferSize)
		{
			return false;
		}
		const int byteIndex = bitIndex >> 3;
		const byte bitOffset = (byte)(bitIndex & 7);
		const uint writeValue = (cacheValue & UINT_WRITE_CACHE_BITS_MASK) << bitOffset;
		const int remainByteCount = bufferSize - byteIndex;
		if (remainByteCount >= 4)
		{
			*((uint*)(buffer + byteIndex)) |= writeValue;
		}
		else
		{
			const byte writeByteCount = (byte)bitCountToByteCount(bitOffset + writeBitCount);
			uint dest = 0;
			memcpy(&dest, buffer + byteIndex, writeByteCount);
			dest |= writeValue;
			memcpy(buffer + byteIndex, &dest, writeByteCount);
		}
		bitIndex = endBitIndex;
		return true;
	}
	inline bool writeCachedUIntBit(char* buffer, const int bufferSize, int& bitIndex, const uint value)
	{
		if (mUIntWriteCache[0] == 0)
		{
			initUIntWriteCache();
		}
		return writeEncodedUIntCache32Bit(buffer, bufferSize, bitIndex, mUIntWriteCache[value]);
	}
	inline bool writeCachedUShortBit(char* buffer, const int bufferSize, int& bitIndex, const ushort value)
	{
		if (mUShortWriteCache[0] == 0)
		{
			initUShortWriteCache();
		}
		return writeEncodedUIntCache32Bit(buffer, bufferSize, bitIndex, mUShortWriteCache[value]);
	}
	// 从当前bitIndex读取一个64位窗口,不移动bitIndex。32位及以下整数的完整编码最多37bit,一次窗口读取即可完成解析
	inline ullong readIntegerBitWindow(const char* buffer, const int bufferSize, const int bitIndex)
	{
		const int byteIndex = bitIndex >> 3;
		const int remainByteCount = bufferSize - byteIndex;
		ullong source = 0;
		if (remainByteCount >= 8)
		{
			source = *((const ullong*)(buffer + byteIndex));
		}
		else if (remainByteCount > 0)
		{
			source = readIntegerTail(buffer + byteIndex, remainByteCount);
		}
		return source >> (bitIndex & 7);
	}
	// 从任意bitIndex读取最多64个连续有效位。仅供列表连续读取缓存使用,必要时会补读第9个字节
	inline ullong readIntegerBitWindow64(const char* buffer, const int bufferSize, const int bitIndex)
	{
		const int byteIndex = bitIndex >> 3;
		const int remainByteCount = bufferSize - byteIndex;
		if (remainByteCount <= 0)
		{
			return 0;
		}
		ullong source = remainByteCount >= 8 ? *((const ullong*)(buffer + byteIndex)) : readIntegerTail(buffer + byteIndex, remainByteCount);
		const byte bitOffset = (byte)(bitIndex & 7);
		ullong value = source >> bitOffset;
		if (bitOffset != 0 && remainByteCount > 8)
		{
			value |= (ullong)(byte)buffer[byteIndex + 8] << (64 - bitOffset);
		}
		return value;
	}
	struct IntegerBitReadCache
	{
		ullong mWindow = 0;
		int mRemainBitCount = 0;
		int mBitIndex = 0;
	};
	inline bool refillIntegerBitReadCache(const char* buffer, const int bufferSize, IntegerBitReadCache& cache)
	{
		const int remainBitCount = bufferSize * 8 - cache.mBitIndex;
		if (remainBitCount <= 0)
		{
			cache.mWindow = 0;
			cache.mRemainBitCount = 0;
			return false;
		}
		cache.mWindow = readIntegerBitWindow64(buffer, bufferSize, cache.mBitIndex);
		cache.mRemainBitCount = remainBitCount > 64 ? 64 : remainBitCount;
		return true;
	}
	inline void consumeIntegerBitReadCache(IntegerBitReadCache& cache, const byte bitCount)
	{
		cache.mWindow = bitCount < 64 ? cache.mWindow >> bitCount : 0;
		cache.mRemainBitCount -= bitCount;
		cache.mBitIndex += bitCount;
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	inline bool readUnsignedIntegerCacheBit(const char* buffer, const int bufferSize, IntegerBitReadCache& cache, T& value)
	{
		static_assert(sizeof(T) <= 4, "readUnsignedIntegerCacheBit only supports <=32bit integer");
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		constexpr byte REMOVE_HIGH_BIT_LIMIT = (1 << sizeof(T)) - 1;
		if (cache.mRemainBitCount < TYPE_LENGTH_MAX_BIT && !refillIntegerBitReadCache(buffer, bufferSize, cache))
		{
			return false;
		}
		ullong window = cache.mWindow;
		byte bitCount = (byte)(window & LENGTH_MAX_VALUE);
		if constexpr (sizeof(T) == 4)
		{
			// uint常见极小值0/1没有数据位,2/3只有1个数据位,直接解码可跳过通用mask/high-bit计算
			if (bitCount <= 1)
			{
				value = (T)bitCount;
				consumeIntegerBitReadCache(cache, TYPE_LENGTH_MAX_BIT);
				return true;
			}
			if (bitCount == 2)
			{
				constexpr byte SHORT_TOTAL_BIT_COUNT = TYPE_LENGTH_MAX_BIT + 1;
				if (cache.mRemainBitCount < SHORT_TOTAL_BIT_COUNT)
				{
					if (!refillIntegerBitReadCache(buffer, bufferSize, cache) || cache.mRemainBitCount < SHORT_TOTAL_BIT_COUNT)
					{
						return false;
					}
					window = cache.mWindow;
				}
				value = (T)(2u | (uint)((window >> TYPE_LENGTH_MAX_BIT) & 1ULL));
				consumeIntegerBitReadCache(cache, SHORT_TOTAL_BIT_COUNT);
				return true;
			}
		}
		if (bitCount == LENGTH_MAX_VALUE)
		{
			++bitCount;
		}
		const bool removeHighBit = bitCount != 0 && bitCount < REMOVE_HIGH_BIT_LIMIT;
		const byte dataBitCount = bitCount == 0 ? 0 : removeHighBit ? bitCount - 1 : bitCount;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
		if (cache.mRemainBitCount < totalBitCount)
		{
			if (!refillIntegerBitReadCache(buffer, bufferSize, cache) || cache.mRemainBitCount < totalBitCount)
			{
				return false;
			}
			window = cache.mWindow;
		}
		ullong unsignedValue = dataBitCount > 0 ? (window >> TYPE_LENGTH_MAX_BIT) & ((1ULL << dataBitCount) - 1ULL) : 0;
		if (removeHighBit)
		{
			unsignedValue |= 1ULL << dataBitCount;
		}
		value = (T)unsignedValue;
		consumeIntegerBitReadCache(cache, totalBitCount);
		return true;
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	inline bool readSignedIntegerCacheBit(const char* buffer, const int bufferSize, IntegerBitReadCache& cache, T& value, const bool needReadSign)
	{
		static_assert(sizeof(T) <= 4, "readSignedIntegerCacheBit only supports <=32bit integer");
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (cache.mRemainBitCount < TYPE_LENGTH_MAX_BIT && !refillIntegerBitReadCache(buffer, bufferSize, cache))
		{
			return false;
		}
		ullong window = cache.mWindow;
		const byte bitCount = (byte)(window & ((1ULL << TYPE_LENGTH_MAX_BIT) - 1ULL));
		const byte dataBitCount = bitCount > 0 ? bitCount - 1 : 0;
		const byte signBitCount = bitCount > 0 && needReadSign ? 1 : 0;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + signBitCount + dataBitCount;
		if (cache.mRemainBitCount < totalBitCount)
		{
			if (!refillIntegerBitReadCache(buffer, bufferSize, cache) || cache.mRemainBitCount < totalBitCount)
			{
				return false;
			}
			window = cache.mWindow;
		}
		if (bitCount == 0)
		{
			value = 0;
			consumeIntegerBitReadCache(cache, totalBitCount);
			return true;
		}
		const bool isNegative = needReadSign && ((window >> TYPE_LENGTH_MAX_BIT) & 1ULL) != 0;
		const byte dataStart = TYPE_LENGTH_MAX_BIT + signBitCount;
		ullong unsignedValue = dataBitCount > 0 ? (window >> dataStart) & ((1ULL << dataBitCount) - 1ULL) : 0;
		unsignedValue |= 1ULL << dataBitCount;
		using UnsignedType = typename ToUnsignedInteger<T>::Type;
		const UnsignedType typedValue = (UnsignedType)unsignedValue;
		value = isNegative ? (T)((UnsignedType)0 - typedValue) : (T)typedValue;
		consumeIntegerBitReadCache(cache, totalBitCount);
		return true;
	}
	// 一次读取整数列表头部的模式位和统一长度位,避免先读1bit模式再单独读取长度位
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	inline bool readSignedIntegerListHeaderBit(const char* buffer, const int bufferSize, int& bitIndex, bool& useUnityBitCount, byte& bitCount)
	{
		if (bitCountToByteCount(bitIndex + 1) > bufferSize)
		{
			return false;
		}
		const ullong window = readIntegerBitWindow(buffer, bufferSize, bitIndex);
		useUnityBitCount = (window & 1ULL) != 0;
		if (!useUnityBitCount)
		{
			++bitIndex;
			bitCount = 0;
			return true;
		}
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + 1 + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		bitCount = (byte)((window >> 1) & ((1ULL << TYPE_LENGTH_MAX_BIT) - 1ULL));
		bitIndex += 1 + TYPE_LENGTH_MAX_BIT;
		return true;
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	inline bool readUnsignedIntegerListHeaderBit(const char* buffer, const int bufferSize, int& bitIndex, bool& useUnityBitCount, byte& bitCount)
	{
		if (bitCountToByteCount(bitIndex + 1) > bufferSize)
		{
			return false;
		}
		const ullong window = readIntegerBitWindow(buffer, bufferSize, bitIndex);
		useUnityBitCount = (window & 1ULL) != 0;
		if (!useUnityBitCount)
		{
			++bitIndex;
			bitCount = 0;
			return true;
		}
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		if (bitCountToByteCount(bitIndex + 1 + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		bitCount = (byte)((window >> 1) & LENGTH_MAX_VALUE);
		if (bitCount == LENGTH_MAX_VALUE)
		{
			++bitCount;
		}
		bitIndex += 1 + TYPE_LENGTH_MAX_BIT;
		return true;
	}
	// 一次写入整数列表的统一长度头部:最低位为1表示统一长度,后面紧跟长度位
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	inline bool writeSignedIntegerListHeaderBit(char* buffer, const int bufferSize, int& bitIndex, const byte bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte HEADER_BIT_COUNT = 1 + TYPE_LENGTH_MAX_BIT;
		if (bitCountToByteCount(bitIndex + HEADER_BIT_COUNT) > bufferSize)
		{
			return false;
		}
		writeIntegerBits(buffer, bufferSize, bitIndex, 1ULL | ((ullong)bitCount << 1), HEADER_BIT_COUNT);
		return true;
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	inline bool writeUnsignedIntegerListHeaderBit(char* buffer, const int bufferSize, int& bitIndex, byte& bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte HEADER_BIT_COUNT = 1 + TYPE_LENGTH_MAX_BIT;
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		if (bitCountToByteCount(bitIndex + HEADER_BIT_COUNT) > bufferSize)
		{
			return false;
		}
		byte writeBitCount = bitCount;
		if (bitCount == 1 << TYPE_LENGTH_MAX_BIT)
		{
			writeBitCount = bitCount - 1;
		}
		else if (bitCount == LENGTH_MAX_VALUE)
		{
			++bitCount;
		}
		writeIntegerBits(buffer, bufferSize, bitIndex, 1ULL | ((ullong)writeBitCount << 1), HEADER_BIT_COUNT);
		return true;
	}
	// 写入无符号整数长度位
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedLengthBit(char* buffer, const int bufferSize, int& bitIndex, byte& bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		// 因为长度位的存储会忽略一个bit,所以当长度位的bit数达到此类型的最大时,会加1
		// 比如byte类型为8bit,长度位存储只用了3bit,只能表示0到7,所以当需要写入的长度位值为8时,就只写7进去,但是实际写入数据时仍然是写入8bit
		// 但是如果写入的长度位值为7,则长度位写入7,数据位仍然是写入8bit
		byte writeBitCount = bitCount;
		if (bitCount == 1 << TYPE_LENGTH_MAX_BIT)
		{
			writeBitCount = bitCount - 1;
		}
		else if (bitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
		{
			writeBitCount = bitCount;
			++bitCount;
		}
		writeByteBits(buffer, bitIndex, writeBitCount, TYPE_LENGTH_MAX_BIT);
		return true;
	}
	// 写入有符号整数长度位
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedLengthBit(char* buffer, const int bufferSize, int& bitIndex, const byte bitCount)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		// 先写入TYPE_LENGTH_MAX_BIT个bit
		writeByteBits(buffer, bitIndex, bitCount, TYPE_LENGTH_MAX_BIT);
		return true;
	}
	MICRO_LEGEND_FRAME_API ushort crc16(ushort crc, const char* buffer, int len, int bufferOffset = 0);
	inline ushort crc16Byte(const ushort crc, const byte data) { return (ushort)((crc >> 8) ^ crc16_table[(crc ^ data) & 0xFF]); }
	template<typename T>
	void zeroArray(T* ptr, const int size) { memset(ptr, 0, sizeof(T) * size); }
	MICRO_LEGEND_FRAME_API bool readBuffer(const char* buffer, int bufferSize, int& index, char* dest, int destSize, int readSize);
	MICRO_LEGEND_FRAME_API bool writeBuffer(char* buffer, int bufferSize, int& destOffset, const char* source, int writeSize);
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool read(const char* buffer, const int bufferSize, int& index, T& value)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return false;
		}
		value = *((T*)(buffer + index));
		index += valueSize;
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr T read(const char* buffer, const int bufferSize, int& index)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return 0;
		}
		const T value = *((T*)(buffer + index));
		index += valueSize;
		return value;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr T readInverse(const char* buffer, const int bufferSize, int& index)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return 0;
		}
		T finalValue = (T)0;
		char* ptr = (char*)&finalValue;
		FOR_INVERSE(valueSize)
		{
			ptr[i] = buffer[index++];
		}
		return finalValue;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool readInverse(const char* buffer, const int bufferSize, int& index, T& value)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return false;
		}
		char* ptr = (char*)&value;
		FOR_INVERSE(valueSize)
		{
			ptr[i] = buffer[index++];
		}
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void readArray(const char* buffer, const int bufferSize, int& index, T* dest, const int arrayLength)
	{
		FOR(arrayLength)
		{
			dest[i] = read<T>(buffer, bufferSize, index);
		}
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void readArrayInverse(const char* buffer, const int bufferSize, int& index, T* dest, const int arrayLength)
	{
		FOR(arrayLength)
		{
			dest[i] = readInverse<T>(buffer, bufferSize, index);
		}
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool write(char* buffer, const int bufferSize, int& index, const T value)
	{
		constexpr int writeSize = sizeof(T);
		if (bufferSize < index + writeSize)
		{
			return false;
		}
		*((T*)(buffer + index)) = value;
		index += writeSize;
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool writeInverse(char* buffer, const int bufferSize, int& index, const T value)
	{
		constexpr int writeSize = sizeof(T);
		if (bufferSize < index + writeSize)
		{
			return false;
		}
		const char* ptr = (char*)&value;
		FOR_INVERSE(writeSize)
		{
			buffer[index++] = ptr[i];
		}
		return true;
	}
	inline bool readBoolBit(const char* buffer, const int bufferSize, int& bitIndex, bool& value)
	{
		// bool固定读取1位
		if (bitCountToByteCount(bitIndex + 1) > bufferSize)
		{
			return false;
		}
		value = getBufferBit(buffer, bitIndex++);
		return true;
	}
	MICRO_LEGEND_FRAME_API bool readBufferBit(const char* buffer, int bufferSize, int& bitIndex, char* dest, int destSize, int readSize);
	// 可以按位读取带符号的整数
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedIntegerBit(const char* buffer, const int bufferSize, int& bitIndex, T& value, bool needReadSign)
	{
		value = 0;
		// 32位及以下整数的完整编码最多37bit,一次读取64位窗口即可完成长度位、符号位和数据位解析
		if constexpr (sizeof(T) <= 4)
		{
			constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
			if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
			{
				return false;
			}
			const ullong window = readIntegerBitWindow(buffer, bufferSize, bitIndex);
			const byte bitCount = (byte)(window & ((1ULL << TYPE_LENGTH_MAX_BIT) - 1ULL));
			if (bitCount == 0)
			{
				bitIndex += TYPE_LENGTH_MAX_BIT;
				return true;
			}
			const byte dataBitCount = bitCount - 1;
			const byte signBitCount = needReadSign ? 1 : 0;
			const byte totalBitCount = TYPE_LENGTH_MAX_BIT + signBitCount + dataBitCount;
			if (bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
			{
				return false;
			}
			const bool isNegative = needReadSign && ((window >> TYPE_LENGTH_MAX_BIT) & 1ULL) != 0;
			const byte dataStart = TYPE_LENGTH_MAX_BIT + signBitCount;
			ullong unsignedValue = dataBitCount > 0 ? (window >> dataStart) & ((1ULL << dataBitCount) - 1ULL) : 0;
			unsignedValue |= 1ULL << dataBitCount;
			using UnsignedType = typename ToUnsignedInteger<T>::Type;
			const UnsignedType typedValue = (UnsignedType)unsignedValue;
			value = isNegative ? (T)((UnsignedType)0 - typedValue) : (T)typedValue;
			bitIndex += totalBitCount;
			return true;
		}
		// 64位整数也先读取一次窗口。常见小值的长度位、符号位、数据位可在一个64位窗口内直接完成解析
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		const ullong window = readIntegerBitWindow(buffer, bufferSize, bitIndex);
		const byte bitCount = (byte)(window & ((1ULL << TYPE_LENGTH_MAX_BIT) - 1ULL));
		if (bitCount == 0)
		{
			bitIndex += TYPE_LENGTH_MAX_BIT;
			return true;
		}
		const byte dataBitCount = bitCount - 1;
		const byte signBitCount = needReadSign ? 1 : 0;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + signBitCount + dataBitCount;
		if (bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
		{
			return false;
		}
		const bool isNegative = needReadSign && ((window >> TYPE_LENGTH_MAX_BIT) & 1ULL) != 0;
		ullong unsignedValue = 0;
		if (totalBitCount <= 64)
		{
			if (dataBitCount > 0)
			{
				unsignedValue = (window >> (TYPE_LENGTH_MAX_BIT + signBitCount)) & ((1ULL << dataBitCount) - 1ULL);
			}
			bitIndex += totalBitCount;
		}
		else
		{
			bitIndex += TYPE_LENGTH_MAX_BIT + signBitCount;
			if (dataBitCount > 0)
			{
				unsignedValue = readIntegerBits(buffer, bufferSize, bitIndex, dataBitCount);
			}
		}
		unsignedValue |= 1ULL << dataBitCount;
		using UnsignedType = typename ToUnsignedInteger<T>::Type;
		const UnsignedType typedValue = (UnsignedType)unsignedValue;
		value = isNegative ? (T)((UnsignedType)0 - typedValue) : (T)typedValue;
		return true;
	}
	// 可以按位读取无符号的整数
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedIntegerBit(const char* buffer, const int bufferSize, int& bitIndex, T& value)
	{
		value = 0;
		if constexpr (sizeof(T) <= 4)
		{
			constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
			constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
			constexpr byte REMOVE_HIGH_BIT_LIMIT = (1 << sizeof(T)) - 1;
			if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
			{
				return false;
			}
			const ullong window = readIntegerBitWindow(buffer, bufferSize, bitIndex);
			byte bitCount = (byte)(window & LENGTH_MAX_VALUE);
			if (bitCount == LENGTH_MAX_VALUE)
			{
				++bitCount;
			}
			if (bitCount == 0)
			{
				bitIndex += TYPE_LENGTH_MAX_BIT;
				return true;
			}
			const bool removeHighBit = bitCount < REMOVE_HIGH_BIT_LIMIT;
			const byte dataBitCount = removeHighBit ? bitCount - 1 : bitCount;
			const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
			if (bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
			{
				return false;
			}
			ullong unsignedValue = dataBitCount > 0 ? (window >> TYPE_LENGTH_MAX_BIT) & ((1ULL << dataBitCount) - 1ULL) : 0;
			if (removeHighBit)
			{
				unsignedValue |= 1ULL << dataBitCount;
			}
			value = (T)unsignedValue;
			bitIndex += totalBitCount;
			return true;
		}
		// 64位无符号整数先一次读取长度窗口。编码总长度不超过64bit时直接从同一窗口完成解析
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
		{
			return false;
		}
		const ullong window = readIntegerBitWindow(buffer, bufferSize, bitIndex);
		byte bitCount = (byte)(window & LENGTH_MAX_VALUE);
		if (bitCount == LENGTH_MAX_VALUE)
		{
			++bitCount;
		}
		if (bitCount == 0)
		{
			bitIndex += TYPE_LENGTH_MAX_BIT;
			return true;
		}
		constexpr byte FULL_BIT_COUNT = sizeof(T) * 8;
		const bool removeHighBit = bitCount < FULL_BIT_COUNT;
		const byte dataBitCount = removeHighBit ? bitCount - 1 : bitCount;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
		if (bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
		{
			return false;
		}
		ullong unsignedValue = 0;
		if (totalBitCount <= 64)
		{
			if (dataBitCount > 0)
			{
				unsignedValue = (window >> TYPE_LENGTH_MAX_BIT) & ((1ULL << dataBitCount) - 1ULL);
			}
			bitIndex += totalBitCount;
		}
		else
		{
			bitIndex += TYPE_LENGTH_MAX_BIT;
			if (dataBitCount > 0)
			{
				unsignedValue = readIntegerBits(buffer, bufferSize, bitIndex, dataBitCount);
			}
		}
		if (removeHighBit)
		{
			unsignedValue |= 1ULL << dataBitCount;
		}
		value = (T)unsignedValue;
		return true;
	}
	MICRO_LEGEND_FRAME_API bool readFloatBit(const char* buffer, int bufferSize, int& bitIndex, float& value, bool needReadSign, const int precision = 3);
	MICRO_LEGEND_FRAME_API bool readDoubleBit(const char* buffer, int bufferSize, int& bitIndex, double& value, bool needReadSign, const int precision = 4);
	// int固定参数列表使用独立的out-of-line核心,避免大型模板在每个调用点重复展开导致MSVC代码布局变差
	MICRO_LEGEND_FRAME_API bool readSignedIntPointerListBit(const char* buffer, int bufferSize, int& bitIndex, int** list, int count, bool needReadSign);
	template<int Count>
	bool readSignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, int*>& list, const bool needReadSign)
	{
		return readSignedIntPointerListBit(buffer, bufferSize, bitIndex, list.data(), Count, needReadSign);
	}
	// llong固定参数列表同样使用out-of-line核心,重点优化GUID/唯一ID等常见的64位参数组
	MICRO_LEGEND_FRAME_API bool readSignedLLongPointerListBit(const char* buffer, int bufferSize, int& bitIndex, llong** list, int count, bool needReadSign);
	template<int Count>
	bool readSignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, llong*>& list, const bool needReadSign)
	{
		return readSignedLLongPointerListBit(buffer, bufferSize, bitIndex, list.data(), Count, needReadSign);
	}
	template<typename T, int Count, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, T*>& list, const bool needReadSign)
	{
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readSignedIntegerListHeaderBit<T>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		// 使用统一的长度位
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + ((needReadSign ? 1 : 0) + bitCount) * Count) > bufferSize)
			{
				return false;
			}
			// 统一长度模式下每个元素一次整块读取,避免按8bit循环读取
			using UnsignedType = typename ToUnsignedInteger<T>::Type;
			const byte signBitCount = needReadSign ? 1 : 0;
			const byte elementBitCount = bitCount + signBitCount;
			FOR(Count)
			{
				bool isNegative = false;
				ullong unsignedValue;
				if (elementBitCount <= 64)
				{
					const ullong packedValue = readIntegerBits(buffer, bufferSize, bitIndex, elementBitCount);
					isNegative = needReadSign && (packedValue & 1ULL) != 0;
					unsignedValue = packedValue >> signBitCount;
				}
				else
				{
					isNegative = getBufferBit(buffer, bitIndex++);
					unsignedValue = readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
				}
				const UnsignedType typedValue = (UnsignedType)unsignedValue;
				*list[i] = isNegative ? (T)((UnsignedType)0 - typedValue) : (T)typedValue;
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			if constexpr (sizeof(T) <= 4)
			{
				IntegerBitReadCache cache;
				cache.mBitIndex = bitIndex;
				FOR(Count)
				{
					if (!readSignedIntegerCacheBit(buffer, bufferSize, cache, *list[i], needReadSign))
					{
						bitIndex = cache.mBitIndex;
						return false;
					}
				}
				bitIndex = cache.mBitIndex;
			}
			else
			{
				FOR(Count)
				{
					if (!readSignedIntegerBit(buffer, bufferSize, bitIndex, *list[i], needReadSign))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool readSignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<T>& list, const bool needReadSign)
	{
		// 读元素个数位
		ushort count = 0;
		if (!readUnsignedIntegerBit(buffer, bufferSize, bitIndex, count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		list.clear();
		list.resize(count);

		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readSignedIntegerListHeaderBit<T>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		// 使用统一的长度位
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + ((needReadSign ? 1 : 0) + bitCount) * count) > bufferSize)
			{
				return false;
			}
			// 统一长度模式下每个元素一次整块读取,避免按8bit循环读取
			using UnsignedType = typename ToUnsignedInteger<T>::Type;
			const byte signBitCount = needReadSign ? 1 : 0;
			const byte elementBitCount = bitCount + signBitCount;
			FOR(count)
			{
				bool isNegative = false;
				ullong unsignedValue;
				if (elementBitCount <= 64)
				{
					const ullong packedValue = readIntegerBits(buffer, bufferSize, bitIndex, elementBitCount);
					isNegative = needReadSign && (packedValue & 1ULL) != 0;
					unsignedValue = packedValue >> signBitCount;
				}
				else
				{
					isNegative = getBufferBit(buffer, bitIndex++);
					unsignedValue = readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
				}
				const UnsignedType typedValue = (UnsignedType)unsignedValue;
				list[i] = isNegative ? (T)((UnsignedType)0 - typedValue) : (T)typedValue;
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			if constexpr (sizeof(T) <= 4)
			{
				IntegerBitReadCache cache;
				cache.mBitIndex = bitIndex;
				FOR(count)
				{
					if (!readSignedIntegerCacheBit(buffer, bufferSize, cache, list[i], needReadSign))
					{
						bitIndex = cache.mBitIndex;
						return false;
					}
				}
				bitIndex = cache.mBitIndex;
			}
			else
			{
				FOR(count)
				{
					if (!readSignedIntegerBit(buffer, bufferSize, bitIndex, list[i], needReadSign))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	// uint固定参数列表使用独立的out-of-line核心,复用已验证的ShortCode FastPath并避免大型模板重复展开
	MICRO_LEGEND_FRAME_API bool readUnsignedIntPointerListBit(const char* buffer, int bufferSize, int& bitIndex, uint** list, int count);
	template<int Count>
	bool readUnsignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, uint*>& list)
	{
		return readUnsignedIntPointerListBit(buffer, bufferSize, bitIndex, list.data(), Count);
	}
	template<typename T, int Count, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, T*>& list)
	{
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readUnsignedIntegerListHeaderBit<T>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		// 使用统一的长度位
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + bitCount * Count) > bufferSize)
			{
				return false;
			}
			// 统一长度模式下每个元素一次整块读取,避免按8bit循环读取
			FOR(Count)
			{
				*list[i] = (T)readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			if constexpr (sizeof(T) <= 4)
			{
				IntegerBitReadCache cache;
				cache.mBitIndex = bitIndex;
				FOR(Count)
				{
					if (!readUnsignedIntegerCacheBit(buffer, bufferSize, cache, *list[i]))
					{
						bitIndex = cache.mBitIndex;
						return false;
					}
				}
				bitIndex = cache.mBitIndex;
			}
			else
			{
				FOR(Count)
				{
					if (!readUnsignedIntegerBit(buffer, bufferSize, bitIndex, *list[i]))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	// uint动态列表同样使用out-of-line核心,避免Vector<uint>大型模板在每个调用点重复展开
	MICRO_LEGEND_FRAME_API bool readUnsignedIntVectorBit(const char* buffer, int bufferSize, int& bitIndex, Vector<uint>& list);
	inline bool readUnsignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<uint>& list)
	{
		return readUnsignedIntVectorBit(buffer, bufferSize, bitIndex, list);
	}
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool readUnsignedIntegerListBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<T>& list)
	{
		// 读元素个数位
		ushort count = 0;
		if (!readUnsignedIntegerBit(buffer, bufferSize, bitIndex, count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		list.clear();
		list.resize(count);

		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readUnsignedIntegerListHeaderBit<T>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		// 使用统一的长度位
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + bitCount * count) > bufferSize)
			{
				return false;
			}
			// 统一长度模式下每个元素一次整块读取,避免按8bit循环读取
			FOR(count)
			{
				list[i] = (T)readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
			}
		}
		// 每个元素使用独立的长度位
		else
		{
			if constexpr (sizeof(T) <= 4)
			{
				IntegerBitReadCache cache;
				cache.mBitIndex = bitIndex;
				FOR(count)
				{
					if (!readUnsignedIntegerCacheBit(buffer, bufferSize, cache, list[i]))
					{
						bitIndex = cache.mBitIndex;
						return false;
					}
				}
				bitIndex = cache.mBitIndex;
			}
			else
			{
				FOR(count)
				{
					if (!readUnsignedIntegerBit(buffer, bufferSize, bitIndex, list[i]))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	MICRO_LEGEND_FRAME_API bool readFloatListBit(const char* buffer, int bufferSize, int& bitIndex, Vector<float>& list, bool needReadSign, int precision = 3);
	MICRO_LEGEND_FRAME_API bool readFloatListBit(const char* buffer, int bufferSize, int& bitIndex, float** list, int count, bool needReadSign, int precision = 3);
	template<int Count>
	bool readFloatListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, float*>& list, const bool needReadSign, const int precision = 3)
	{
		return readFloatListBit(buffer, bufferSize, bitIndex, list.data(), Count, needReadSign, precision);
	}
	MICRO_LEGEND_FRAME_API bool readDoubleListBit(const char* buffer, int bufferSize, int& bitIndex, Vector<double>& list, bool needReadSign, int precision = 4);
	MICRO_LEGEND_FRAME_API bool readDoubleListBit(const char* buffer, int bufferSize, int& bitIndex, double** list, int count, bool needReadSign, int precision = 4);
	template<int Count>
	bool readDoubleListBit(const char* buffer, const int bufferSize, int& bitIndex, Array<Count, double*>& list, const bool needReadSign, const int precision = 4)
	{
		return readDoubleListBit(buffer, bufferSize, bitIndex, list.data(), Count, needReadSign, precision);
	}
	inline bool writeBoolBit(char* buffer, const int bufferSize, int& bitIndex, const bool value)
	{
		if (bitCountToByteCount(bitIndex + 1) > bufferSize)
		{
			return false;
		}
		// 固定只写入1位
		if (value)
		{
			setBufferBitOne(buffer, bitIndex);
		}
		++bitIndex;
		return true;
	}
	MICRO_LEGEND_FRAME_API void fillZeroToByteEnd(char* buffer, int& bitIndex);
	MICRO_LEGEND_FRAME_API void writeFloatIntegerBits32(char* buffer, const int bufferSize, int& bitIndex, uint value, byte writeBitCount);
	MICRO_LEGEND_FRAME_API bool writeFloatCachedSignedIntBit(char* buffer, const int bufferSize, int& bitIndex, int value, byte bitCount, bool use32BitWrite);
	MICRO_LEGEND_FRAME_API bool writeBufferBit(char* buffer, int bufferSize, int& bitIndex, const char* source, int writeSize);
	// 可以按位写入带符号的整数
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerBit(char* buffer, const int bufferSize, int& bitIndex, T value, const bool needWriteSign)
	{
		using UnsignedType = typename ToUnsignedInteger<T>::Type;
		const bool isNegative = value < 0;
		const UnsignedType rawValue = (UnsignedType)value;
		const UnsignedType unsignedValue = isNegative ? (UnsignedType)0 - rawValue : rawValue;
		const byte bitCount = generateBitCount(unsignedValue);
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte MAX_SUPPORTED_BIT_COUNT = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		// signed协议的长度字段没有unsigned的“最大长度特殊值”编码,因此各有符号类型的最小值无法无歧义表示
		if ((unsignedValue != 0 && bitCount == 0) || bitCount > MAX_SUPPORTED_BIT_COUNT)
		{
			return false;
		}
		// 32位及以下整数将长度位、符号位、数据位一次打包后写入,避免多次位操作
		if constexpr (sizeof(T) <= 4)
		{
			if (bitCount == 0)
			{
				if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
				{
					return false;
				}
				writeIntegerBits(buffer, bufferSize, bitIndex, 0, TYPE_LENGTH_MAX_BIT);
				return true;
			}
			const byte dataBitCount = bitCount - 1;
			const byte signBitCount = needWriteSign ? 1 : 0;
			const byte totalBitCount = TYPE_LENGTH_MAX_BIT + signBitCount + dataBitCount;
			const byte checkBitCount = TYPE_LENGTH_MAX_BIT + 1 + dataBitCount;
			if (bitCountToByteCount(bitIndex + checkBitCount) > bufferSize)
			{
				return false;
			}
			ullong packedValue = bitCount;
			if (needWriteSign && isNegative)
			{
				packedValue |= 1ULL << TYPE_LENGTH_MAX_BIT;
			}
			const ullong dataValue = dataBitCount > 0 ? (ullong)unsignedValue & ((1ULL << dataBitCount) - 1ULL) : 0;
			packedValue |= dataValue << (TYPE_LENGTH_MAX_BIT + signBitCount);
			writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, totalBitCount);
			return true;
		}
		// 64位整数的常见小值也将长度、符号、数据打包到一次64位写入中;较大值则只把长度和符号头部合并
		if (bitCount == 0)
		{
			if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
			{
				return false;
			}
			writeIntegerBits(buffer, bufferSize, bitIndex, 0, TYPE_LENGTH_MAX_BIT);
			return true;
		}
		const byte dataBitCount = bitCount - 1;
		const byte signBitCount = needWriteSign ? 1 : 0;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + signBitCount + dataBitCount;
		if (bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
		{
			return false;
		}
		const ullong dataValue = dataBitCount > 0 ? (ullong)unsignedValue & ((1ULL << dataBitCount) - 1ULL) : 0;
		ullong headerValue = bitCount;
		if (needWriteSign && isNegative)
		{
			headerValue |= 1ULL << TYPE_LENGTH_MAX_BIT;
		}
		if (totalBitCount <= 64)
		{
			const ullong packedValue = headerValue | (dataValue << (TYPE_LENGTH_MAX_BIT + signBitCount));
			writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, totalBitCount);
		}
		else
		{
			writeIntegerBits(buffer, bufferSize, bitIndex, headerValue, TYPE_LENGTH_MAX_BIT + signBitCount);
			if (dataBitCount > 0)
			{
				writeIntegerBits(buffer, bufferSize, bitIndex, dataValue, dataBitCount);
			}
		}
		return true;
	}
	// 可以按位写入无符号的整数
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerBit(char* buffer, const int bufferSize, int& bitIndex, T value)
	{
		if constexpr (sizeof(T) == 2)
		{
			return writeCachedUShortBit(buffer, bufferSize, bitIndex, (ushort)value);
		}
		if constexpr (sizeof(T) == 4)
		{
			if ((uint)value < UINT_WRITE_CACHE_PRODUCTION_COUNT)
			{
				return writeCachedUIntBit(buffer, bufferSize, bitIndex, (uint)value);
			}
		}
		const byte bitCount = generateBitCount(value);
		if ((value != 0 && bitCount == 0) || bitCount > sizeof(value) * 8)
		{
			return false;
		}
		if constexpr (sizeof(T) <= 4)
		{
			constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
			constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
			constexpr byte REMOVE_HIGH_BIT_LIMIT = (1 << sizeof(T)) - 1;
			byte writeLength = bitCount;
			byte dataLength = bitCount;
			if (bitCount == 1 << TYPE_LENGTH_MAX_BIT)
			{
				writeLength = bitCount - 1;
			}
			else if (bitCount == LENGTH_MAX_VALUE)
			{
				++dataLength;
			}
			if (value == 0)
			{
				if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
				{
					return false;
				}
				writeIntegerBits(buffer, bufferSize, bitIndex, writeLength, TYPE_LENGTH_MAX_BIT);
				return true;
			}
			const bool removeHighBit = dataLength < REMOVE_HIGH_BIT_LIMIT;
			const byte dataBitCount = removeHighBit ? dataLength - 1 : dataLength;
			const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
			if (bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
			{
				return false;
			}
			const ullong dataValue = dataBitCount > 0 ? (ullong)value & ((1ULL << dataBitCount) - 1ULL) : 0;
			const ullong packedValue = (ullong)writeLength | (dataValue << TYPE_LENGTH_MAX_BIT);
			writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, totalBitCount);
			return true;
		}
		// 64位无符号整数同样优先把长度和数据打包到一个64位写入窗口
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		byte writeLength = bitCount;
		byte dataLength = bitCount;
		if (bitCount == 1 << TYPE_LENGTH_MAX_BIT)
		{
			writeLength = bitCount - 1;
		}
		else if (bitCount == LENGTH_MAX_VALUE)
		{
			++dataLength;
		}
		if (value == 0)
		{
			if (bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
			{
				return false;
			}
			writeIntegerBits(buffer, bufferSize, bitIndex, writeLength, TYPE_LENGTH_MAX_BIT);
			return true;
		}
		constexpr byte FULL_BIT_COUNT = sizeof(T) * 8;
		const bool removeHighBit = dataLength < FULL_BIT_COUNT;
		const byte dataBitCount = removeHighBit ? dataLength - 1 : dataLength;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
		if (bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
		{
			return false;
		}
		ullong dataValue = (ullong)value;
		if (removeHighBit && dataBitCount > 0)
		{
			dataValue &= (1ULL << dataBitCount) - 1ULL;
		}
		if (totalBitCount <= 64)
		{
			const ullong packedValue = (ullong)writeLength | (dataValue << TYPE_LENGTH_MAX_BIT);
			writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, totalBitCount);
		}
		else
		{
			writeIntegerBits(buffer, bufferSize, bitIndex, writeLength, TYPE_LENGTH_MAX_BIT);
			if (dataBitCount > 0)
			{
				writeIntegerBits(buffer, bufferSize, bitIndex, dataValue, dataBitCount);
			}
		}
		return true;
	}
	MICRO_LEGEND_FRAME_API bool writeFloatListBit(char* buffer, int bufferSize, int& bitIndex, const float* list, int count, bool needWriteCount, bool needWriteSign, int precision = 3);
	template<int Count>
	bool writeFloatListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, float>& list, const bool needWriteSign, const int precision = 3)
	{
		return writeFloatListBit(buffer, bufferSize, bitIndex, list.data(), Count, false, needWriteSign, precision);
	}
	inline bool writeFloatListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<float>& list, const bool needWriteSign, const int precision = 3)
	{
		return writeFloatListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true, needWriteSign, precision);
	}
	MICRO_LEGEND_FRAME_API bool writeDoubleListBit(char* buffer, int bufferSize, int& bitIndex, const double* list, const int count, bool needWriteCount, bool needWriteSign, int precision = 4);
	template<int Count>
	bool writeDoubleListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, double>& list, const bool needWriteSign, const int precision = 4)
	{
		return writeDoubleListBit(buffer, bufferSize, bitIndex, list.data(), Count, false, needWriteSign, precision);
	}
	inline bool writeDoubleListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<double>& list, const bool needWriteSign, const int precision = 4)
	{
		return writeDoubleListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true, needWriteSign, precision);
	}
	// 可以按位写入带符号的整数的列表
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const T* list, const int count, const bool needWriteCount, const bool needWriteSign)
	{
		// 动态列表的数量使用ushort存储,超过上限时必须在写入任何数据前直接失败,不能静默截断。
		if (count < 0 || (needWriteCount && count > USHRT_MAX))
		{
			return false;
		}
		// 写入长度
		if (needWriteCount && !writeUnsignedIntegerBit(buffer, bufferSize, bitIndex, (ushort)count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		// 计算最大绝对值时使用无符号运算,避免对INT_MIN/LLONG_MIN取负产生溢出
		using UnsignedType = typename ToUnsignedInteger<T>::Type;
		UnsignedType maxAbsValue = 0;
		FOR(count)
		{
			const UnsignedType rawValue = (UnsignedType)list[i];
			const UnsignedType absValue = list[i] < 0 ? (UnsignedType)0 - rawValue : rawValue;
			if (maxAbsValue < absValue)
			{
				maxAbsValue = absValue;
			}
		}
		const byte maxBitCount = generateBitCount(maxAbsValue);
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(T)];
		constexpr byte MAX_SUPPORTED_BIT_COUNT = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		if (maxBitCount > MAX_SUPPORTED_BIT_COUNT)
		{
			return false;
		}
		// 如果使用一个统一的位数来表示写入位个数所占用的总位数
		int bitCountUnity;
		if (maxBitCount > 0)
		{
			if (needWriteSign)
			{
				bitCountUnity = (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT;
			}
			else
			{
				bitCountUnity = maxBitCount * count + TYPE_LENGTH_MAX_BIT;
			}
		}
		else
		{
			bitCountUnity = TYPE_LENGTH_MAX_BIT;
		}
		// 如果每个值都使用自己的实际位数来表示写入位个数所占用的总位数
		// 先加上每个元素的长度位的位数量
		int bitCountSingle = TYPE_LENGTH_MAX_BIT * count;
		// 写入独立长度所占空间小于统一个数时才会继续计算值所占的空间,如果长度部分已经大于了,则不需要再继续计算了
		if (bitCountSingle < bitCountUnity)
		{
			FOR(count)
			{
				T thisAbsValue = abs(list[i]);
				// 值为0时不会写入符号位和数据,不为0时才会写入
				if (thisAbsValue > 0)
				{
					// 每个元素绝对值所占用的位数,最高位固定是1,所以减去1位
					bitCountSingle += generateBitCount((typename ToUnsignedInteger<T>::Type)thisAbsValue) - 1;
					// 还要加上一个符号位
					if (needWriteSign)
					{
						bitCountSingle += 1;
					}
					if (bitCountSingle > bitCountUnity)
					{
						break;
					}
				}
			}
		}

		// 使用统一的长度位占空间更小
		if (bitCountSingle >= bitCountUnity)
		{
			// 模式位1和统一长度位一次写入
			if (!writeSignedIntegerListHeaderBit<T>(buffer, bufferSize, bitIndex, maxBitCount))
			{
				return false;
			}
			// 长度位为0,则不需要再继续写入
			if (maxBitCount == 0)
			{
				return true;
			}
			int willWriteBitCount;
			if (needWriteSign)
			{
				willWriteBitCount = (maxBitCount + 1) * count;
			}
			else
			{
				willWriteBitCount = maxBitCount * count;
			}
			if (bitCountToByteCount(bitIndex + willWriteBitCount) > bufferSize)
			{
				return false;
			}
			// 统一长度模式下将多个元素打包到64bit后一次写入,减少writeIntegerBits调用次数
			using UnsignedType = typename ToUnsignedInteger<T>::Type;
			const byte signBitCount = needWriteSign ? 1 : 0;
			const byte elementBitCount = maxBitCount + signBitCount;
			if (elementBitCount <= 64)
			{
				ullong packedValue = 0;
				byte packedBitCount = 0;
				FOR(count)
				{
					const T value = list[i];
					const bool isNegative = value < 0;
					const UnsignedType rawValue = (UnsignedType)value;
					const UnsignedType unsignedValue = isNegative ? (UnsignedType)0 - rawValue : rawValue;
					const ullong elementValue = ((ullong)unsignedValue << signBitCount) | (needWriteSign && isNegative ? 1ULL : 0ULL);
					if ((int)packedBitCount + elementBitCount > 64)
					{
						writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, packedBitCount);
						packedValue = 0;
						packedBitCount = 0;
					}
					packedValue |= elementValue << packedBitCount;
					packedBitCount = (byte)(packedBitCount + elementBitCount);
					if (packedBitCount == 64)
					{
						writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, 64);
						packedValue = 0;
						packedBitCount = 0;
					}
				}
				if (packedBitCount > 0)
				{
					writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, packedBitCount);
				}
			}
			else
			{
				FOR(count)
				{
					const T value = list[i];
					const bool isNegative = value < 0;
					const UnsignedType rawValue = (UnsignedType)value;
					const UnsignedType unsignedValue = isNegative ? (UnsignedType)0 - rawValue : rawValue;
					if (isNegative)
					{
						setBufferBitOne(buffer, bitIndex);
					}
					++bitIndex;
					writeIntegerBits(buffer, bufferSize, bitIndex, (ullong)unsignedValue, maxBitCount);
				}
			}
		}
		// 使用每个元素独立的长度位占用空间更小
		else
		{
			// 写入0表示使用独立的长度位,只是下标跳一位,不会实际写入
			++bitIndex;
			FOR(count)
			{
				writeSignedIntegerBit(buffer, bufferSize, bitIndex, list[i], needWriteSign);
			}
		}
		return true;
	}
	// 可以按位写入带符号的整数的列表,Array类型
	template<typename T, int Count, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, T>& list, const bool needWriteSign)
	{
		return writeSignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), false, needWriteSign);
	}
	// 可以按位写入带符号的整数的列表,Vector类型
	template<typename T, typename TypeCheck = typename IsPodSignedIntegerType<T>::mType>
	bool writeSignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<T>& list, const bool needWriteSign)
	{
		return writeSignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true, needWriteSign);
	}
	// 可以按位写入无符号的整数的列表
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const T* list, const int count, const bool needWriteCount)
	{
		// 动态列表的数量使用ushort存储,超过上限时必须在写入任何数据前直接失败,不能静默截断。
		if (count < 0 || (needWriteCount && count > USHRT_MAX))
		{
			return false;
		}
		// 写入长度
		if (needWriteCount && !writeUnsignedIntegerBit(buffer, bufferSize, bitIndex, (ushort)count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}

		// 计算出最大值的位数量,所有的值都使用这个位数量来存储
		byte maxBitCount = generateBitCount(getListMax(list, count));
		constexpr byte TYPE_LENGTH_MAX_BIT = UNSIGNED_LENGTH_MAX_BIT[sizeof(T)];
		int bitCountUnity;
		// 这里的判断与writeUnsignedLengthBit中的逻辑有关,writeUnsignedLengthBit会计算出写入到长度位的值,以及实际需要写入的数据位的位数
		if (maxBitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
		{
			bitCountUnity = (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT;
		}
		else
		{
			bitCountUnity = maxBitCount * count + TYPE_LENGTH_MAX_BIT;
		}
		// 如果使用一个统一的位数来表示写入位个数所占用的总位数
		// 如果每个值都使用自己的实际位数来表示写入位个数所占用的总位数
		// 先加上每个元素的长度位的位数量
		int bitCountSingle = TYPE_LENGTH_MAX_BIT * count;
		// 写入独立长度所占空间小于统一个数时才会继续计算值所占的空间,如果长度部分已经大于了,则不需要再继续计算了
		if (bitCountSingle < bitCountUnity)
		{
			// 每个元素绝对值所占用的位数,最高位固定是1,所以减去1位
			FOR(count)
			{
				T thisValue = list[i];
				if (thisValue > 0)
				{
					byte thisBitCount = generateBitCount(thisValue);
					if (thisBitCount == (1 << TYPE_LENGTH_MAX_BIT) - 1)
					{
						++thisBitCount;
					}
					bitCountSingle += thisBitCount - 1;
					if (bitCountSingle > bitCountUnity)
					{
						break;
					}
				}
			}
		}

		// 使用统一的长度位占空间更小
		if (bitCountSingle >= bitCountUnity)
		{
			// 模式位1和统一长度位一次写入,并保持原有无符号长度位的特殊值规则
			if (!writeUnsignedIntegerListHeaderBit<T>(buffer, bufferSize, bitIndex, maxBitCount))
			{
				return false;
			}
			// 长度位为0,则不需要再继续写入
			if (maxBitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + maxBitCount * count) > bufferSize)
			{
				return false;
			}
			// 统一长度模式下将多个元素打包到64bit后一次写入,减少writeIntegerBits调用次数
			ullong packedValue = 0;
			byte packedBitCount = 0;
			FOR(count)
			{
				if ((int)packedBitCount + maxBitCount > 64)
				{
					writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, packedBitCount);
					packedValue = 0;
					packedBitCount = 0;
				}
				packedValue |= (ullong)list[i] << packedBitCount;
				packedBitCount = (byte)(packedBitCount + maxBitCount);
				if (packedBitCount == 64)
				{
					writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, 64);
					packedValue = 0;
					packedBitCount = 0;
				}
			}
			if (packedBitCount > 0)
			{
				writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, packedBitCount);
			}
		}
		// 使用每个元素独立的长度位占用空间更小
		else
		{
			// 写入0表示使用独立的长度位,只是下标跳一位,不会实际写入
			++bitIndex;
			FOR(count)
			{
				writeUnsignedIntegerBit(buffer, bufferSize, bitIndex, list[i]);
			}
		}
		return true;
	}
	// 可以按位写入无符号的整数的列表,Array类型
	template<typename T, int Count, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Array<Count, T>& list)
	{
		return writeUnsignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), false);
	}
	// 可以按位写入无符号的整数的列表,Vector类型
	template<typename T, typename TypeCheck = typename IsPodUnsignedIntegerType<T>::mType>
	bool writeUnsignedIntegerListBit(char* buffer, const int bufferSize, int& bitIndex, const Vector<T>& list)
	{
		return writeUnsignedIntegerListBit(buffer, bufferSize, bitIndex, list.data(), list.size(), true);
	}
	inline bool writeVector2(char* buffer, const int bufferSize, int& index, const Vector2 value)
	{
		return write(buffer, bufferSize, index, value.x) && write(buffer, bufferSize, index, value.y);
	}
	inline bool writeVector2Int(char* buffer, const int bufferSize, int& index, const Vector2Int value)
	{
		return write(buffer, bufferSize, index, value.x) && write(buffer, bufferSize, index, value.y);
	}
	inline bool writeVector2IntInverse(char* buffer, const int bufferSize, int& index, const Vector2Int value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) && writeInverse(buffer, bufferSize, index, value.y);
	}
	inline bool writeVector3(char* buffer, const int bufferSize, int& index, const Vector3& value)
	{
		return write(buffer, bufferSize, index, value.x) &&
			   write(buffer, bufferSize, index, value.y) &&
			   write(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector3Int(char* buffer, const int bufferSize, int& index, const Vector3Int& value)
	{
		return write(buffer, bufferSize, index, value.x) &&
			   write(buffer, bufferSize, index, value.y) &&
			   write(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector3Inverse(char* buffer, const int bufferSize, int& index, const Vector3& value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) &&
			   writeInverse(buffer, bufferSize, index, value.y) &&
			   writeInverse(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector3IntInverse(char* buffer, const int bufferSize, int& index, const Vector3Int& value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) &&
			   writeInverse(buffer, bufferSize, index, value.y) &&
			   writeInverse(buffer, bufferSize, index, value.z);
	}
	inline bool writeVector4(char* buffer, const int bufferSize, int& index, const Vector4& value)
	{
		return write(buffer, bufferSize, index, value.x) &&
			   write(buffer, bufferSize, index, value.y) &&
			   write(buffer, bufferSize, index, value.z) &&
			   write(buffer, bufferSize, index, value.w);
	}
	inline bool writeVector4Inverse(char* buffer, const int bufferSize, int& index, const Vector4& value)
	{
		return writeInverse(buffer, bufferSize, index, value.x) &&
			   writeInverse(buffer, bufferSize, index, value.y) &&
			   writeInverse(buffer, bufferSize, index, value.z) &&
			   writeInverse(buffer, bufferSize, index, value.w);
	}
	inline bool writeColor(char* buffer, int bufferSize, int& index, const Color& value)
	{
		return write(buffer, bufferSize, index, value.r) &&
			   write(buffer, bufferSize, index, value.g) &&
			   write(buffer, bufferSize, index, value.b) &&
			   write(buffer, bufferSize, index, value.a);
	}
	inline bool writeColorInverse(char* buffer, const int bufferSize, int& index, const Color& value)
	{
		return writeInverse(buffer, bufferSize, index, value.r) &&
			   writeInverse(buffer, bufferSize, index, value.g) &&
			   writeInverse(buffer, bufferSize, index, value.b) &&
			   writeInverse(buffer, bufferSize, index, value.a);
	}
	inline bool readVector2(const char* buffer, const int bufferSize, int& index, Vector2& value)
	{
		return read(buffer, bufferSize, index, value.x) && read(buffer, bufferSize, index, value.y);
	}
	inline bool readVector2Inverse(const char* buffer, const int bufferSize, int& index, Vector2& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) && readInverse(buffer, bufferSize, index, value.y);
	}
	inline bool readVector2Int(const char* buffer, const int bufferSize, int& index, Vector2Int& value)
	{
		return read(buffer, bufferSize, index, value.x) && read(buffer, bufferSize, index, value.y);
	}
	inline bool readVector2IntInverse(const char* buffer, const int bufferSize, int& index, Vector2Int& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) && readInverse(buffer, bufferSize, index, value.y);
	}
	inline bool readVector3(const char* buffer, const int bufferSize, int& index, Vector3& value)
	{
		return read(buffer, bufferSize, index, value.x) &&
			   read(buffer, bufferSize, index, value.y) &&
			   read(buffer, bufferSize, index, value.z);
	}
	inline bool readVector3Int(const char* buffer, const int bufferSize, int& index, Vector3Int& value)
	{
		return read(buffer, bufferSize, index, value.x) && 
			   read(buffer, bufferSize, index, value.y) &&
			   read(buffer, bufferSize, index, value.z);
	}
	inline bool readVector3Inverse(const char* buffer, const int bufferSize, int& index, Vector3& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) &&
			   readInverse(buffer, bufferSize, index, value.y) &&
			   readInverse(buffer, bufferSize, index, value.z);
	}
	inline bool readVector4(const char* buffer, const int bufferSize, int& index, Vector4& value)
	{
		return read(buffer, bufferSize, index, value.x) &&
			   read(buffer, bufferSize, index, value.y) &&
			   read(buffer, bufferSize, index, value.z) &&
			   read(buffer, bufferSize, index, value.w);
	}
	inline bool readVector4Inverse(const char* buffer, const int bufferSize, int& index, Vector4& value)
	{
		return readInverse(buffer, bufferSize, index, value.x) &&
			   readInverse(buffer, bufferSize, index, value.y) &&
			   readInverse(buffer, bufferSize, index, value.z) &&
			   readInverse(buffer, bufferSize, index, value.w);
	}
	inline bool readColor(const char* buffer, const int bufferSize, int& index, Color& value)
	{
		return read(buffer, bufferSize, index, value.r) &&
			   read(buffer, bufferSize, index, value.g) &&
			   read(buffer, bufferSize, index, value.b) &&
			   read(buffer, bufferSize, index, value.a);
	}
	inline bool readColorInverse(const char* buffer, const int bufferSize, int& index, Color& value)
	{
		return readInverse(buffer, bufferSize, index, value.r) &&
			   readInverse(buffer, bufferSize, index, value.g) &&
			   readInverse(buffer, bufferSize, index, value.b) &&
			   readInverse(buffer, bufferSize, index, value.a);
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool writeArray(char* buffer, const int bufferSize, int& index, T* valueArray, const int arrayLength)
	{
		FOR(arrayLength)
		{
			if (!write(buffer, bufferSize, index, valueArray[i]))
			{
				return false;
			}
		}
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr bool writeArrayInverse(char* buffer, const int bufferSize, int& index, T* valueArray, const int arrayLength)
	{
		FOR(arrayLength)
		{
			if (!writeInverse(buffer, bufferSize, index, valueArray[i]))
			{
				return false;
			}
		}
		return true;
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void swapByte(T& value, const int pos0, const int pos1)
	{
		const char byte0 = getByte(value, pos0);
		const char byte1 = getByte(value, pos1);
		setByte(value, byte0, pos1);
		setByte(value, byte1, pos0);
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodType<T>::mValue>>
	constexpr void inverseByte(T& value)
	{
		constexpr int typeSize = sizeof(T);
		constexpr int halfSize = typeSize >> 1;
		FOR(halfSize)
		{
			swapByte(value, i, typeSize - 1 - i);
		}
	}
	inline void setString(char* buffer, const int bufferSize, const string& str)
	{
		const int length = (int)str.length();
		MEMCPY(buffer, bufferSize, str.c_str(), length);
		buffer[length] = '\0';
	}
}

using BinaryUtility::hasBit;
using BinaryUtility::setBitOne;
using BinaryUtility::setBitZero;
using BinaryUtility::getByte;
using BinaryUtility::bitCountToByteCount;
using BinaryUtility::setBit;
using BinaryUtility::getBit;
using BinaryUtility::setString;
using BinaryUtility::crc16;
using BinaryUtility::writeBuffer;
using BinaryUtility::readBuffer;
using BinaryUtility::writeVector2;
using BinaryUtility::writeVector3;
using BinaryUtility::writeVector4;
using BinaryUtility::writeBoolBit;
using BinaryUtility::writeSignedIntegerBit;
using BinaryUtility::writeUnsignedIntegerBit;
using BinaryUtility::writeBufferBit;
using BinaryUtility::writeSignedIntegerListBit;
using BinaryUtility::writeUnsignedIntegerListBit;
using BinaryUtility::writeFloatListBit;
using BinaryUtility::writeDoubleListBit;
using BinaryUtility::readBoolBit;
using BinaryUtility::readBufferBit;
using BinaryUtility::readSignedIntegerBit;
using BinaryUtility::readUnsignedIntegerBit;
using BinaryUtility::readSignedIntegerListBit;
using BinaryUtility::readUnsignedIntegerListBit;
using BinaryUtility::readFloatBit;
using BinaryUtility::readDoubleBit;
using BinaryUtility::readFloatListBit;
using BinaryUtility::readDoubleListBit;
using BinaryUtility::mStringType;
using BinaryUtility::mBoolType;
using BinaryUtility::mCharType;
using BinaryUtility::mWCharType;
using BinaryUtility::mByteType;
using BinaryUtility::mShortType;
using BinaryUtility::mUShortType;
using BinaryUtility::mIntType;
using BinaryUtility::mUIntType;
using BinaryUtility::mFloatType;
using BinaryUtility::mLLongType;
using BinaryUtility::mULLongType;
using BinaryUtility::mBoolPtrType;
using BinaryUtility::mCharPtrType;
using BinaryUtility::mBytePtrType;
using BinaryUtility::mShortPtrType;
using BinaryUtility::mUShortPtrType;
using BinaryUtility::mIntPtrType;
using BinaryUtility::mUIntPtrType;
using BinaryUtility::mFloatPtrType;
using BinaryUtility::mLLongPtrType;
using BinaryUtility::mULLongPtrType;
using BinaryUtility::mBoolListType;
using BinaryUtility::mCharListType;
using BinaryUtility::mByteListType;
using BinaryUtility::mShortListType;
using BinaryUtility::mUShortListType;
using BinaryUtility::mIntListType;
using BinaryUtility::mUIntListType;
using BinaryUtility::mFloatListType;
using BinaryUtility::mLLongListType;
using BinaryUtility::mULLongListType;
using BinaryUtility::mVector2Type;
using BinaryUtility::mVector2IntType;
using BinaryUtility::mVector2UIntType;
using BinaryUtility::mVector2ShortType;
using BinaryUtility::mVector2UShortType;
using BinaryUtility::mVector3Type;
using BinaryUtility::mVector3IntType;
using BinaryUtility::mVector2ListType;
using BinaryUtility::mVector2IntListType;
using BinaryUtility::mVector3ListType;
using BinaryUtility::mVector3IntListType;
using BinaryUtility::readVector2;
using BinaryUtility::readVector3;
using BinaryUtility::readVector4;
using BinaryUtility::writeColor;
using BinaryUtility::readInverse;
using BinaryUtility::read;
using BinaryUtility::write;
using BinaryUtility::writeInverse;
using BinaryUtility::inverseByte;
using BinaryUtility::swapByte;
using BinaryUtility::copyBits;
using BinaryUtility::getLowestBit;
using BinaryUtility::getHighestBit;
using BinaryUtility::setByte;
using BinaryUtility::getByte;
using BinaryUtility::getBits;
using BinaryUtility::generateBitCount;
using BinaryUtility::readColor;