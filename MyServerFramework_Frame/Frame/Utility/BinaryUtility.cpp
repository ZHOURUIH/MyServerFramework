#include "FrameHeader.h"

namespace BinaryUtility
{
	const int mStringType = (int)typeid(string).hash_code();
	const int mBoolType = (int)typeid(bool).hash_code();
	const int mCharType = (int)typeid(char).hash_code();
	const int mWCharType = (int)typeid(wchar_t).hash_code();
	const int mByteType = (int)typeid(unsigned char).hash_code();
	const int mShortType = (int)typeid(short).hash_code();
	const int mUShortType = (int)typeid(ushort).hash_code();
	const int mIntType = (int)typeid(int).hash_code();
	const int mUIntType = (int)typeid(uint).hash_code();
	const int mFloatType = (int)typeid(float).hash_code();
	const int mLLongType = (int)typeid(llong).hash_code();
	const int mULLongType = (int)typeid(ullong).hash_code();
	const int mBoolsType = (int)typeid(bool*).hash_code();
	const int mCharsType = (int)typeid(char*).hash_code();
	const int mBytesType = (int)typeid(byte*).hash_code();
	const int mShortPtrType = (int)typeid(short*).hash_code();
	const int mUShortPtrType = (int)typeid(ushort*).hash_code();
	const int mIntPtrType = (int)typeid(int*).hash_code();
	const int mUIntPtrType = (int)typeid(uint*).hash_code();
	const int mFloatPtrType = (int)typeid(float*).hash_code();
	const int mLLongPtrType = (int)typeid(llong*).hash_code();
	const int mULLongPtrType = (int)typeid(ullong*).hash_code();
	const int mBoolListType = (int)typeid(Vector<bool>).hash_code();
	const int mCharListType = (int)typeid(Vector<char>).hash_code();
	const int mByteListType = (int)typeid(Vector<byte>).hash_code();
	const int mShortListType = (int)typeid(Vector<short>).hash_code();
	const int mUShortListType = (int)typeid(Vector<ushort>).hash_code();
	const int mIntListType = (int)typeid(Vector<int>).hash_code();
	const int mUIntListType = (int)typeid(Vector<uint>).hash_code();
	const int mFloatListType = (int)typeid(Vector<float>).hash_code();
	const int mLLongListType = (int)typeid(Vector<llong>).hash_code();
	const int mULLongListType = (int)typeid(Vector<ullong>).hash_code();
	const int mVector2Type = (int)typeid(Vector2).hash_code();
	const int mVector3Type = (int)typeid(Vector3).hash_code();
	const int mVector2IntType = (int)typeid(Vector2Int).hash_code();
	const int mVector3IntType = (int)typeid(Vector3Int).hash_code();
	const int mVector2UIntType = (int)typeid(Vector2UInt).hash_code();
	const int mVector2ShortType = (int)typeid(Vector2Short).hash_code();
	const int mVector2UShortType = (int)typeid(Vector2UShort).hash_code();
	const int mVector2ListType = (int)typeid(Vector<Vector2>).hash_code();
	const int mVector2IntListType = (int)typeid(Vector<Vector2Int>).hash_code();
	const int mVector3ListType = (int)typeid(Vector<Vector3>).hash_code();
	const int mVector3IntListType = (int)typeid(Vector<Vector3Int>).hash_code();
	byte mBitCountTable[65536];
	ThreadLock mBitCountTableLock;
	uint mUIntWriteCache[UINT_WRITE_CACHE_PRODUCTION_COUNT];
	ThreadLock mUIntWriteCacheLock;
	alignas(64) uint mUShortWriteCache[USHORT_WRITE_CACHE_COUNT];
	ThreadLock mUShortWriteCacheLock;
	const ushort crc16_table[256]
	{
		0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
		0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
		0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
		0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
		0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
		0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
		0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
		0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
		0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
		0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
		0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
		0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
		0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
		0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
		0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
		0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
		0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
		0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
		0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
		0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
		0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
		0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
		0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
		0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
		0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
		0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
		0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
		0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
		0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
		0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
		0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
		0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
	};
	alignas(64) static ushort crc16_slice1[256];
	alignas(64) static ushort crc16_slice2[256];
	alignas(64) static ushort crc16_slice3[256];
	class CRC16SliceTableInitializer
	{
	public:
		CRC16SliceTableInitializer()
		{
			for (int i = 0; i < 256; ++i)
			{
				const ushort crc1 = crc16Byte(0, (byte)i);
				crc16_slice1[i] = crc16Byte(crc1, 0);
				crc16_slice2[i] = crc16Byte(crc16_slice1[i], 0);
				crc16_slice3[i] = crc16Byte(crc16_slice2[i], 0);
			}
		}
	};
	static CRC16SliceTableInitializer gCRC16SliceTableInitializer;
	ushort crc16(ushort crc, const char* buffer, const int len, const int bufferOffset)
	{
		if (len <= 0)
		{
			return crc;
		}
		const byte* data = (const byte*)buffer + bufferOffset;
		int remain = len;
		while (remain >= 4)
		{
			crc = (ushort)(
				crc16_slice3[(crc ^ data[0]) & 0xFF] ^
				crc16_slice2[((crc >> 8) ^ data[1]) & 0xFF] ^
				crc16_slice1[data[2]] ^
				crc16_table[data[3]]);
			data += 4;
			remain -= 4;
		}
		while (remain > 0)
		{
			crc = crc16Byte(crc, *data);
			++data;
			--remain;
		}
		return crc;
	}

	bool readBuffer(const char* buffer, const int bufferSize, int& index, char* dest, const int destSize, const int readSize)
	{
		if (bufferSize < index + readSize)
		{
			return false;
		}
		if (readSize > 0)
		{
			MEMCPY(dest, destSize, buffer + index, readSize);
			index += readSize;
		}
		return true;
	}

	bool readSignedIntPointerListBit(const char* buffer, const int bufferSize, int& bitIndex, int** list, const int count, const bool needReadSign)
	{
		if (count == 0)
		{
			return true;
		}
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readSignedIntegerListHeaderBit<int>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			const byte signBitCount = needReadSign ? 1 : 0;
			const byte elementBitCount = bitCount + signBitCount;
			if (bitCountToByteCount(bitIndex + elementBitCount * count) > bufferSize)
			{
				return false;
			}
			FOR(count)
			{
				const ullong packedValue = readIntegerBits(buffer, bufferSize, bitIndex, elementBitCount);
				const bool isNegative = needReadSign && (packedValue & 1ULL) != 0;
				const uint unsignedValue = (uint)(packedValue >> signBitCount);
				*list[i] = isNegative ? (int)(0u - unsignedValue) : (int)unsignedValue;
			}
		}
		else
		{
			IntegerBitReadCache cache;
			cache.mBitIndex = bitIndex;
			FOR(count)
			{
				int value = 0;
				if (!readSignedIntegerCacheBit(buffer, bufferSize, cache, value, needReadSign))
				{
					bitIndex = cache.mBitIndex;
					return false;
				}
				*list[i] = value;
			}
			bitIndex = cache.mBitIndex;
		}
		return true;
	}

	bool readSignedLLongPointerListBit(const char* buffer, const int bufferSize, int& bitIndex, llong** list, const int count, const bool needReadSign)
	{
		if (count == 0)
		{
			return true;
		}
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readSignedIntegerListHeaderBit<llong>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			const byte signBitCount = needReadSign ? 1 : 0;
			const byte elementBitCount = bitCount + signBitCount;
			if (bitCountToByteCount(bitIndex + elementBitCount * count) > bufferSize)
			{
				return false;
			}
			FOR(count)
			{
				bool isNegative = false;
				ullong unsignedValue = 0;
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
				*list[i] = isNegative ? (llong)(0ULL - unsignedValue) : (llong)unsignedValue;
			}
		}
		else
		{
			FOR(count)
			{
				llong value = 0;
				if (!readSignedIntegerBit(buffer, bufferSize, bitIndex, value, needReadSign))
				{
					return false;
				}
				*list[i] = value;
			}
		}
		return true;
	}

	bool readUnsignedIntPointerListBit(const char* buffer, const int bufferSize, int& bitIndex, uint** list, const int count)
	{
		if (count == 0)
		{
			return true;
		}
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readUnsignedIntegerListHeaderBit<uint>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
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
			FOR(count)
			{
				*list[i] = (uint)readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
			}
		}
		else
		{
			IntegerBitReadCache cache;
			cache.mBitIndex = bitIndex;
			FOR(count)
			{
				uint value = 0;
				if (!readUnsignedIntegerCacheBit(buffer, bufferSize, cache, value))
				{
					bitIndex = cache.mBitIndex;
					return false;
				}
				*list[i] = value;
			}
			bitIndex = cache.mBitIndex;
		}
		return true;
	}

	bool readUnsignedIntVectorBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<uint>& list)
	{
		ushort count = 0;
		if (!readUnsignedIntegerBit(buffer, bufferSize, bitIndex, count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}
		// V37: 不再clear后重新resize。动态协议列表通常会复用同一个Vector,元素数量也经常稳定,
		// 直接resize可以在count不变时完整复用现有storage,随后解码会覆盖每个元素。
		// 为保持V36失败语义,只有失败或bitCount==0时才补齐原本clear+resize产生的0值。
		list.resize(count);
		uint* output = list.data();
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readUnsignedIntegerListHeaderBit<uint>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			memset(output, 0, sizeof(uint) * count);
			return false;
		}
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				memset(output, 0, sizeof(uint) * count);
				return true;
			}
			if (bitCountToByteCount(bitIndex + bitCount * count) > bufferSize)
			{
				memset(output, 0, sizeof(uint) * count);
				return false;
			}
			FOR(count)
			{
				output[i] = (uint)readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
			}
		}
		else
		{
			IntegerBitReadCache cache;
			cache.mBitIndex = bitIndex;
			FOR(count)
			{
				if (!readUnsignedIntegerCacheBit(buffer, bufferSize, cache, output[i]))
				{
					memset(output + i, 0, sizeof(uint) * (count - i));
					bitIndex = cache.mBitIndex;
					return false;
				}
			}
			bitIndex = cache.mBitIndex;
		}
		return true;
	}

	bool readFloatBit(const char* buffer, const int bufferSize, int& bitIndex, float& value, const bool needReadSign, const int precision)
	{
		// 浮点数按照一定精度转换为整数再写入
		int intValue = 0;
		const bool result = readSignedIntegerBit(buffer, bufferSize, bitIndex, intValue, needReadSign);
		value = intValue * inversePow10(precision);
		return result;
	}

	bool readDoubleBit(const char* buffer, const int bufferSize, int& bitIndex, double& value, const bool needReadSign, const int precision)
	{
		// 浮点数按照一定精度转换为整数再写入
		llong llongValue = 0;
		const bool result = readSignedIntegerBit(buffer, bufferSize, bitIndex, llongValue, needReadSign);
		value = llongValue * inversePow10LLong(precision);
		return result;
	}

	bool readFloatListBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<float>& list, const bool needReadSign, const int precision)
	{
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
		const float powValue = inversePow10(precision);
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readSignedIntegerListHeaderBit<int>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
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
			const byte signBitCount = needReadSign ? 1 : 0;
			const byte elementBitCount = bitCount + signBitCount;
			FOR(count)
			{
				const ullong packedValue = readIntegerBits(buffer, bufferSize, bitIndex, elementBitCount);
				const bool isNegative = needReadSign && (packedValue & 1ULL) != 0;
				const uint unsignedValue = (uint)(packedValue >> signBitCount);
				const int intValue = isNegative ? (int)(0u - unsignedValue) : (int)unsignedValue;
				list[i] = intValue * powValue;
			}
		}
		else
		{
			IntegerBitReadCache cache;
			cache.mBitIndex = bitIndex;
			FOR(count)
			{
				int value = 0;
				if (!readSignedIntegerCacheBit(buffer, bufferSize, cache, value, needReadSign))
				{
					bitIndex = cache.mBitIndex;
					return false;
				}
				list[i] = value * powValue;
			}
			bitIndex = cache.mBitIndex;
		}
		return true;
	}

	bool readFloatListBit(const char* buffer, const int bufferSize, int& bitIndex, float** list, const int count, const bool needReadSign, const int precision)
	{
		if (count == 0)
		{
			return true;
		}
		const float powValue = inversePow10(precision);
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!readSignedIntegerListHeaderBit<int>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
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
			const byte signBitCount = needReadSign ? 1 : 0;
			const byte elementBitCount = bitCount + signBitCount;
			FOR(count)
			{
				const ullong packedValue = readIntegerBits(buffer, bufferSize, bitIndex, elementBitCount);
				const bool isNegative = needReadSign && (packedValue & 1ULL) != 0;
				const uint unsignedValue = (uint)(packedValue >> signBitCount);
				const int intValue = isNegative ? (int)(0u - unsignedValue) : (int)unsignedValue;
				*list[i] = intValue * powValue;
			}
		}
		else
		{
			IntegerBitReadCache cache;
			cache.mBitIndex = bitIndex;
			FOR(count)
			{
				int value = 0;
				if (!readSignedIntegerCacheBit(buffer, bufferSize, cache, value, needReadSign))
				{
					bitIndex = cache.mBitIndex;
					return false;
				}
				*list[i] = value * powValue;
			}
			bitIndex = cache.mBitIndex;
		}
		return true;
	}

	bool readDoubleListBit(const char* buffer, const int bufferSize, int& bitIndex, Vector<double>& list, const bool needReadSign, const int precision)
	{
		// 读取长度
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

		const double powValue = inversePow10LLong(precision);
		// 使用统一的长度位
		if (getBufferBit(buffer, bitIndex++))
		{
			byte bitCount = 0;
			if (!readSignedLengthBit<llong>(buffer, bufferSize, bitIndex, bitCount))
			{
				return false;
			}
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + ((needReadSign ? 1 : 0) + bitCount) * count) > bufferSize)
			{
				return false;
			}
			// 读取所有的值
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(count)
			{
				// 读符号位
				bool isNegative = false;
				if (needReadSign)
				{
					isNegative = getBufferBit(buffer, bitIndex++);
				}

				// 读取值
				llong llongValue = 0;
				FOR_J(byteCount - 1)
				{
					readByteBits(buffer, bitIndex, ((byte*)&llongValue)[j], 8);
				}
				readByteBits(buffer, bitIndex, ((byte*)&(llongValue))[byteCount - 1], bitCount - ((byteCount - 1) << 3));
				list[i] = isNegative ? -llongValue * powValue : llongValue * powValue;
			}
		}
		else
		{
			FOR(count)
			{
				llong value = 0;
				readSignedIntegerBit(buffer, bufferSize, bitIndex, value, needReadSign);
				list[i] = value * powValue;
			}
		}
		return true;
	}

	bool readDoubleListBit(const char* buffer, const int bufferSize, int& bitIndex, double** list, const int count, const bool needReadSign, const int precision)
	{
		if (count == 0)
		{
			return true;
		}

		const double powValue = inversePow10LLong(precision);
		// 使用统一的长度位
		if (getBufferBit(buffer, bitIndex++))
		{
			byte bitCount = 0;
			if (!readSignedLengthBit<llong>(buffer, bufferSize, bitIndex, bitCount))
			{
				return false;
			}
			if (bitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + ((needReadSign ? 1 : 0) + bitCount) * count) > bufferSize)
			{
				return false;
			}
			// 读取所有的值
			const int byteCount = bitCountToByteCount(bitCount);
			FOR(count)
			{
				// 读符号位
				bool isNegative = false;
				if (needReadSign)
				{
					isNegative = getBufferBit(buffer, bitIndex++);
				}

				// 读取值
				llong llongValue = 0;
				FOR_J(byteCount - 1)
				{
					readByteBits(buffer, bitIndex, ((byte*)&(llongValue))[j], 8);
				}
				readByteBits(buffer, bitIndex, ((byte*)&(llongValue))[byteCount - 1], bitCount - ((byteCount - 1) << 3));
				*list[i] = isNegative ? -llongValue * powValue : llongValue * powValue;
			}
		}
		else
		{
			FOR(count)
			{
				llong value = 0;
				readSignedIntegerBit(buffer, bufferSize, bitIndex, value, needReadSign);
				*list[i] = value * powValue;
			}
		}
		return true;
	}

	bool writeBuffer(char* buffer, const int bufferSize, int& destOffset, const char* source, const int writeSize)
	{
		if (writeSize == 0 || source == nullptr)
		{
			return true;
		}
		if (bufferSize < destOffset + writeSize)
		{
			return false;
		}
		if (writeSize > 0)
		{
			MEMCPY(buffer + destOffset, bufferSize - destOffset, source, writeSize);
			destOffset += writeSize;
		}
		return true;
	}

	bool readBufferBit(const char* buffer, const int bufferSize, int& bitIndex, char* dest, const int destSize, const int readSize)
	{
		// 如果不读取任何数据,则不做改变
		if (readSize == 0)
		{
			return true;
		}
		// buffer不能按照位进行读取,但是所以需要将位偏移调整到字节的开头
		int byteIndex = bitCountToByteCount(bitIndex);
		const bool result = readBuffer(buffer, bufferSize, byteIndex, dest, destSize, readSize);
		bitIndex = byteIndex << 3;
		return result;
	}

	bool writeBufferBit(char* buffer, const int bufferSize, int& bitIndex, const char* source, const int writeSize)
	{
		// 如果不写入任何数据,则不做改变
		if (writeSize == 0)
		{
			return true;
		}
		fillZeroToByteEnd(buffer, bitIndex);
		int byteIndex = bitIndex >> 3;
		const bool result = writeBuffer(buffer, bufferSize, byteIndex, source, writeSize);
		bitIndex = byteIndex << 3;
		return result;
	}

	void fillZeroToByteEnd(char* buffer, int& bitIndex)
	{
		const int byteIndex = bitCountToByteCount(bitIndex);
		const int targetBitIndex = byteIndex << 3;
		// 需要将中间空出来的位填充为0
		for (int i = bitIndex; i < targetBitIndex; ++i)
		{
			setBit(buffer[i >> 3], i & 7, 0);
		}
		bitIndex = byteIndex << 3;
	}

	void writeFloatIntegerBits32(char* buffer, const int bufferSize, int& bitIndex, uint value, const byte writeBitCount)
	{
		if (writeBitCount == 0)
		{
			return;
		}
		if (writeBitCount < 32)
		{
			value &= (1u << writeBitCount) - 1u;
		}
		const int byteIndex = bitIndex >> 3;
		const byte bitOffset = (byte)(bitIndex & 7);
		const int remainByteCount = bufferSize - byteIndex;
		const byte writeByteCount = (byte)bitCountToByteCount(bitOffset + writeBitCount);
		const uint writeValue = value << bitOffset;
		if (remainByteCount >= 4)
		{
			*((uint*)(buffer + byteIndex)) |= writeValue;
		}
		else
		{
			uint dest = 0;
			memcpy(&dest, buffer + byteIndex, writeByteCount);
			dest |= writeValue;
			memcpy(buffer + byteIndex, &dest, writeByteCount);
		}
		bitIndex += writeBitCount;
	}
	bool writeFloatCachedSignedIntBit(char* buffer, const int bufferSize, int& bitIndex, const int value, const byte bitCount, const bool use32BitWrite)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(int)];
		constexpr byte MAX_SUPPORTED_BIT_COUNT = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		if (bitCount > MAX_SUPPORTED_BIT_COUNT)
		{
			return false;
		}
		if (bitCount == 0)
		{
			writeIntegerBits(buffer, bufferSize, bitIndex, 0, TYPE_LENGTH_MAX_BIT);
			return true;
		}
		const bool isNegative = value < 0;
		const uint rawValue = (uint)value;
		const uint unsignedValue = isNegative ? 0u - rawValue : rawValue;
		const byte dataBitCount = bitCount - 1;
		ullong packedValue = bitCount | (isNegative ? 1ULL << TYPE_LENGTH_MAX_BIT : 0ULL);
		if (dataBitCount > 0)
		{
			packedValue |= ((ullong)unsignedValue & ((1ULL << dataBitCount) - 1ULL)) << (TYPE_LENGTH_MAX_BIT + 1);
		}
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + bitCount;
		if (use32BitWrite)
		{
			writeFloatIntegerBits32(buffer, bufferSize, bitIndex, (uint)packedValue, totalBitCount);
		}
		else
		{
			writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, totalBitCount);
		}
		return true;
	}

	// 将所有的浮点数都扩大到整数存储
	bool writeFloatListBit(char* buffer, const int bufferSize, int& bitIndex, const float* list, const int count, const bool needWriteCount, const bool needWriteSign, const int precision)
	{
		// 动态列表的数量使用ushort存储,超过上限时必须在写入任何数据前直接失败。
		if (count < 0 || (needWriteCount && count > USHRT_MAX))
		{
			return false;
		}
		if (needWriteCount && !writeUnsignedIntegerBit(buffer, bufferSize, bitIndex, (ushort)count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}
		const int powValue = pow10(precision);
		constexpr int CACHE_COUNT = 64;
		constexpr int PACKED_BIT_COUNT_CACHE_COUNT = 12;
		constexpr byte PACKED_BIT_COUNT_BITS = 5;
		constexpr ullong PACKED_BIT_COUNT_MASK = (1ULL << PACKED_BIT_COUNT_BITS) - 1ULL;
		int valueCache[CACHE_COUNT];
		byte bitCountCache[CACHE_COUNT];
		ullong packedBitCountCache = 0;
		const bool useValueCache = count <= CACHE_COUNT;
		const bool usePackedBitCountCache = needWriteSign && count <= PACKED_BIT_COUNT_CACHE_COUNT;
		byte maxBitCount = 0;
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(int)];
		int bitCountSingle = TYPE_LENGTH_MAX_BIT * count;
		int bitCountUnity = 0;
		if (useValueCache)
		{
			FOR(count)
			{
				const int intValue = round_(list[i] * powValue);
				valueCache[i] = intValue;
				const uint rawValue = (uint)intValue;
				const uint absValue = intValue < 0 ? 0u - rawValue : rawValue;
				const byte thisBitCount = generateBitCount(absValue);
				if (needWriteSign)
				{
					if (usePackedBitCountCache)
					{
						packedBitCountCache |= (ullong)thisBitCount << (i * PACKED_BIT_COUNT_BITS);
					}
					else
					{
						bitCountCache[i] = thisBitCount;
					}
				}
				if (thisBitCount > maxBitCount)
				{
					maxBitCount = thisBitCount;
				}
				if (absValue > 0)
				{
					bitCountSingle += thisBitCount;
				}
			}
			bitCountUnity = maxBitCount > 0 ? (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT : TYPE_LENGTH_MAX_BIT;
		}
		else
		{
			maxBitCount = generateBitCount((uint)round_(getFloatListMaxAbs(list, count) * powValue));
			bitCountUnity = maxBitCount > 0 ? (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT : TYPE_LENGTH_MAX_BIT;
			if (bitCountSingle < bitCountUnity)
			{
				FOR(count)
				{
					const int thisAbsValue = round_(abs(list[i]) * powValue);
					if (thisAbsValue > 0)
					{
						bitCountSingle += generateBitCount((uint)thisAbsValue);
						if (bitCountSingle > bitCountUnity)
						{
							break;
						}
					}
				}
			}
		}
		if (bitCountSingle >= bitCountUnity)
		{
			if (!writeSignedIntegerListHeaderBit<int>(buffer, bufferSize, bitIndex, maxBitCount))
			{
				return false;
			}
			if (maxBitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + (1 + maxBitCount) * count) > bufferSize)
			{
				return false;
			}
			const byte signBitCount = needWriteSign ? 1 : 0;
			const byte elementBitCount = maxBitCount + signBitCount;
			FOR(count)
			{
				const int intValue = useValueCache ? valueCache[i] : round_(list[i] * powValue);
				const bool isNegative = intValue < 0;
				const uint rawValue = (uint)intValue;
				const uint unsignedValue = isNegative ? 0u - rawValue : rawValue;
				const ullong packedValue = ((ullong)unsignedValue << signBitCount) | (needWriteSign && isNegative ? 1ULL : 0ULL);
				writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, elementBitCount);
			}
		}
		else
		{
			++bitIndex;
			if (useValueCache && needWriteSign)
			{
				if (bitCountToByteCount(bitIndex + bitCountSingle) > bufferSize)
				{
					return false;
				}
				const bool use32BitWrite = maxBitCount <= 20;
				FOR(count)
				{
					const byte thisBitCount = usePackedBitCountCache ? (byte)((packedBitCountCache >> (i * PACKED_BIT_COUNT_BITS)) & PACKED_BIT_COUNT_MASK) : bitCountCache[i];
					if (!writeFloatCachedSignedIntBit(buffer, bufferSize, bitIndex, valueCache[i], thisBitCount, use32BitWrite))
					{
						return false;
					}
				}
			}
			else
			{
				FOR(count)
				{
					const int intValue = round_(list[i] * powValue);
					if (!writeSignedIntegerBit(buffer, bufferSize, bitIndex, intValue, needWriteSign))
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	bool writeDoubleListBit(char* buffer, const int bufferSize, int& bitIndex, const double* list, const int count, const bool needWriteCount, const bool needWriteSign, const int precision)
	{
		// 动态列表的数量使用ushort存储,超过上限时必须在写入任何数据前直接失败。
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
		const llong powValue = pow10LLong(precision);
		const byte maxBitCount = generateBitCount((ullong)roundDouble(getDoubleListMaxAbs(list, count) * powValue));
		constexpr byte TYPE_LENGTH_MAX_BIT = SIGNED_LENGTH_MAX_BIT[sizeof(llong)];
		// 如果使用一个统一的位数来表示写入位个数所占用的总位数
		int bitCountUnity;
		if (maxBitCount > 0)
		{
			bitCountUnity = (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT;
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
				const llong thisAbsValue = roundDouble(abs(list[i]) * powValue);
				// 值为0时不会写入符号位和数据,不为0时才会写入
				if (thisAbsValue > 0)
				{
					// 每个元素绝对值所占用的位数,最高位固定是1,所以减去1位,然后还要加上一个符号位
					bitCountSingle += generateBitCount((ullong)thisAbsValue) - 1 + 1;
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
			// 写入1表示使用统一的长度位
			setBufferBitOne(buffer, bitIndex++);
			// 先写入长度位
			if (!writeSignedLengthBit<llong>(buffer, bufferSize, bitIndex, maxBitCount))
			{
				return false;
			}
			// 长度位为0,则不需要再继续写入
			if (maxBitCount == 0)
			{
				return true;
			}
			if (bitCountToByteCount(bitIndex + (1 + maxBitCount) * count) > bufferSize)
			{
				return false;
			}
			const byte byteCount = bitCountToByteCount(maxBitCount);
			FOR(count)
			{
				llong llongValue = roundDouble(list[i] * powValue);
				const bool isNegative = llongValue < 0;
				if (isNegative)
				{
					llongValue = -llongValue;
				}
				if (needWriteSign)
				{
					// 写入符号位
					if (isNegative)
					{
						setBufferBitOne(buffer, bitIndex);
					}
					++bitIndex;
				}
				// 再写入值的所有位
				FOR_J(byteCount - 1)
				{
					writeByteBits(buffer, bitIndex, ((byte*)&llongValue)[j], 8);
				}
				writeByteBits(buffer, bitIndex, ((byte*)&llongValue)[byteCount - 1], maxBitCount - ((byteCount - 1) << 3));
			}
		}
		// 使用每个元素独立的长度位占用空间更小
		else
		{
			// 写入0表示使用独立的长度位,只是下标跳一位,不会实际写入
			++bitIndex;
			FOR(count)
			{
				writeSignedIntegerBit(buffer, bufferSize, bitIndex, roundDouble(list[i] * powValue), needWriteSign);
			}
		}
		return true;
	}
	
	float getFloatListMaxAbs(const float* list, const int count)
	{
		if (count == 0)
		{
			return 0;
		}

		float maxValue = abs(list[0]);
		for (int i = 1; i < count; ++i)
		{
			clampMinRef(maxValue, abs(list[i]));
		}
		return maxValue;
	}

	double getDoubleListMaxAbs(const double* list, const int count)
	{
		if (count == 0)
		{
			return 0;
		}

		double maxValue = abs(list[0]);
		for (int i = 1; i < count; ++i)
		{
			clampMinRef(maxValue, abs(list[i]));
		}
		return maxValue;
	}

	void initBitCountTable()
	{
		THREAD_LOCK(mBitCountTableLock);
		if (mBitCountTable[1] != 0)
		{
			return;
		}
		FOR(65536)
		{
			mBitCountTable[i] = internalGenerateBitCount(i);
		}
	}

	void initUIntWriteCache()
	{
		if (mUIntWriteCache[0] != 0)
		{
			return;
		}
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		mUIntWriteCacheLock.waitForUnlock(__FILE__, __LINE__);
		if (mUIntWriteCache[0] == 0)
		{
			for (uint value = 0; value < UINT_WRITE_CACHE_PRODUCTION_COUNT; ++value)
			{
				mUIntWriteCache[value] = encodeUIntWriteCacheValue(value);
			}
		}
		mUIntWriteCacheLock.unlock();
	}

	void initUShortWriteCache()
	{
		if (mUShortWriteCache[0] != 0)
		{
			return;
		}
		if (mBitCountTable[1] == 0)
		{
			initBitCountTable();
		}
		mUShortWriteCacheLock.waitForUnlock(__FILE__, __LINE__);
		if (mUShortWriteCache[0] == 0)
		{
			for (uint value = 0; value < USHORT_WRITE_CACHE_COUNT; ++value)
			{
				mUShortWriteCache[value] = encodeUShortWriteCacheValue((ushort)value);
			}
		}
		mUShortWriteCacheLock.unlock();
	}

	byte internalGenerateBitCount(const ushort value)
	{
		if (value == 0)
		{
			return 0;
		}
		// 从高到低遍历每一位,找到最高位1的下标
		FOR(16)
		{
			if (getBit(value, 15 - i) == 1)
			{
				return 16 - i;
			}
		}
		return 0;
	}

	void logErrorInternal(string&& info) 
	{
		ERROR(info);
	}
}