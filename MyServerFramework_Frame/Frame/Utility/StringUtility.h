#pragma once

#include "BinaryUtility.h"
#include "ThreadLock.h"
#include "CharArrayScopeThread.h"
#include "Vector.h"
#include "ArrayList.h"
#include "MyString.h"

namespace StringUtility
{
	MICRO_LEGEND_FRAME_API extern Array<20000, string> mIntString;
	MICRO_LEGEND_FRAME_API extern const Array<11, llong> POWER_INT_10;
	MICRO_LEGEND_FRAME_API extern const Array<6, float> INVERSE_POWER_INT_10;
	MICRO_LEGEND_FRAME_API extern const Array<19, llong> POWER_LLONG_10;
	MICRO_LEGEND_FRAME_API extern const Array<10, double> INVERSE_POWER_LLONG_10;
	MICRO_LEGEND_FRAME_API extern const char BOM[4];
	MICRO_LEGEND_FRAME_API extern string ChineseSpace;
	MICRO_LEGEND_FRAME_API extern byte alphabet_map[];
	MICRO_LEGEND_FRAME_API extern byte reverse_map[];
	MICRO_LEGEND_FRAME_API extern ThreadLock mIntStringLock;
#ifdef LINUX
	MICRO_LEGEND_FRAME_API extern ThreadLock mSetLocaleLock;		// linux下由于在转换编码时需要先设置全局的编码,所以需要加锁
#endif
	//------------------------------------------------------------------------------------------------------------------------------
	// private
	MICRO_LEGEND_FRAME_API void initIntToString();
	// 为了避免在头文件中包含MathUtility,在此封装一下要调的函数
	MICRO_LEGEND_FRAME_API int greaterPower2(int value);
	//------------------------------------------------------------------------------------------------------------------------------
	// public
	MICRO_LEGEND_FRAME_API string removeSuffix(const string& str);
	// 去掉开始的所有指定字符,直到遇到不是该字符的为止
	MICRO_LEGEND_FRAME_API void removeStartAll(string& stream, char key);
	// 去掉第一个出现的指定字符
	MICRO_LEGEND_FRAME_API void removeStart(string& stream, char key);
	// 去掉最后出现的所有指定字符,从后往前,直到遇到不是该字符的为止
	MICRO_LEGEND_FRAME_API void removeLastAll(string& stream, char key);
	// 去掉最后一个出现的指定字符
	MICRO_LEGEND_FRAME_API void removeLast(string& stream, char key);
	// 去掉最后一个逗号
	void removeLastComma(string& stream) { removeLast(stream, ','); }
	MICRO_LEGEND_FRAME_API int strlength(const char* str, int maxLength);
	MICRO_LEGEND_FRAME_API int strlength(const char* str);
	// 查找str中指定key的数量
	MICRO_LEGEND_FRAME_API int findCharCount(const string& str, char key);
	MICRO_LEGEND_FRAME_API int strchar(const char* str, char key, int startIndex = 0, int length = 0);
	MICRO_LEGEND_FRAME_API string getFileName(const string& str);
	MICRO_LEGEND_FRAME_API string getFileNameNoSuffix(const string& str, bool removePath);
	MICRO_LEGEND_FRAME_API string getFirstFolderName(const string& dir);
	MICRO_LEGEND_FRAME_API string removeFirstPath(const string& dir);
	MICRO_LEGEND_FRAME_API string getFilePath(const string& dir);
	MICRO_LEGEND_FRAME_API string getFileSuffix(const string& fileName);
	MICRO_LEGEND_FRAME_API string removeStartString(const string& fileName, const string& startStr);
	MICRO_LEGEND_FRAME_API string removeEndString(const string& fileName, const string& endStr);
	string replaceSuffix(const string& fileName, const string& suffix) { return getFileNameNoSuffix(fileName, false) + suffix; }
	// 获得字符串最后不是数字的下标
	MICRO_LEGEND_FRAME_API int getLastNotNumberPos(const string& str);
	// 获得字符串结尾的数字
	MICRO_LEGEND_FRAME_API int getLastNumber(const string& str);
	// 获得去除末尾数字以后的字符串
	string getNotNumberSubString(const string& str) { return str.substr(0, getLastNotNumberPos(str) + 1); }
	MICRO_LEGEND_FRAME_API int getLastChar(const char* str, char value);
	MICRO_LEGEND_FRAME_API int getLastNotChar(const char* str, char value);
	MICRO_LEGEND_FRAME_API void splitLine(const char* str, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void splitLine(const char* str, string* stringBuffer, int bufferSize, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void split(const char* str, char key, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API int split(const char* str, char key, string* stringBuffer, int bufferSize, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void split(const char* str, const char* key, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void split(const string& str, const char* key, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API int split(const char* str, const char* key, string* stringBuffer, int bufferSize, bool removeEmpty = true);
	// 将字符串全部转为小写再查找
	MICRO_LEGEND_FRAME_API bool findStringLower(const string& res, const string& sub, int* pos = nullptr, int startIndex = 0, bool direction = true);
	// 可指定从后或者从头查找子字符串
	MICRO_LEGEND_FRAME_API bool findString(const string& res, const char* dst, int* pos = nullptr, int startIndex = 0, bool direction = true);
	MICRO_LEGEND_FRAME_API bool findString(const char* res, const char* dst, int* pos = nullptr, int startIndex = 0, bool direction = true);
	MICRO_LEGEND_FRAME_API int findStringPos(const char* res, const char* dst, int startIndex = 0, bool direction = true);
	MICRO_LEGEND_FRAME_API int findStringPos(const string& res, const string& dst, int startIndex = 0, bool direction = true);
	// 将str中的[begin,end)替换为reStr
	template<int Length>
	void replace(MyString<Length>& str, const int begin, const int end, const char* reStr)
	{
		replace(str.toBuffer(), Length, begin, end, reStr);
	}
	MICRO_LEGEND_FRAME_API void replace(char* str, int strBufferSize, int begin, int end, const char* reStr);
	MICRO_LEGEND_FRAME_API void replace(string& str, int begin, int end, const string& reStr);
	template<int Length>
	void replaceAll(MyString<Length>& str, const char* key, const char* newWord)
	{
		replaceAll(str.toBuffer(), Length, key, newWord);
	}
	MICRO_LEGEND_FRAME_API void replaceAll(char* str, int strBufferSize, const char* key, const char* newWord);
	MICRO_LEGEND_FRAME_API void replaceAll(string& str, const string& key, const string& newWord);
	MICRO_LEGEND_FRAME_API void replaceAll(string& str, char key, char newWord);
	MICRO_LEGEND_FRAME_API void removeAll(string& str, char value);
	MICRO_LEGEND_FRAME_API void removeAll(string& str, char value0, char value1);
	template<int Length>
	bool removeString(MyString<Length>& str, const char* subString)
	{
		int subPos = 0;
		if (!findString(str.str(), subString, &subPos, 0))
		{
			return false;
		}
		// 从子字符串的位置,后面的数据覆盖前面的数据
		int subLength = strlength(subString);
		memmove(str.toBuffer() + subPos, str.toBuffer() + subPos + subLength, Length - subLength - subPos);
		return true;
	}
	MICRO_LEGEND_FRAME_API bool removeString(char* str, int length, const char* subString);
	// 判断oriString是否以pattern结尾,sensitive为是否大小写敏感
	MICRO_LEGEND_FRAME_API bool endWith(const char* oriString, const char* pattern, bool sensitive = true);
	MICRO_LEGEND_FRAME_API bool endWith(const string& oriString, const char* pattern, bool sensitive = true);
	// 判断oriString是否以pattern开头,sensitive为是否大小写敏感
	MICRO_LEGEND_FRAME_API bool startWith(const char* oriString, const char* pattern, bool sensitive = true);
	MICRO_LEGEND_FRAME_API bool startWith(const string& oriString, const char* pattern, bool sensitive = true);
	// 将source拼接到destBuffer后面
	MICRO_LEGEND_FRAME_API void strcat_s(char* destBuffer, int size, const char* source);
	MICRO_LEGEND_FRAME_API void strcat_s(char* destBuffer, int size, const char* source, int length);
	template<int Length>
	void strcat_s(MyString<Length>& destBuffer, const string& source)
	{
		const int destIndex = destBuffer.length();
		destBuffer.copy(destIndex, source);
		destBuffer[destIndex + (int)source.length()] = '\0';
	}
	template<int Length>
	void strcat_s(MyString<Length>& destBuffer, const string& source, const int length)
	{
		const int destIndex = destBuffer.length();
		destBuffer.copy(destIndex, source, length);
		destBuffer[destIndex + length] = '\0';
	}
	template<int Length>
	void strcat_s(MyString<Length>& destBuffer, const char* source)
	{
		const int destIndex = destBuffer.length();
		const int length = strlength(source);
		destBuffer.copy(destIndex, source, length);
		destBuffer[destIndex + length] = '\0';
	}
	template<int Length>
	void strcat_s(MyString<Length>& destBuffer, const char* source, const int length)
	{
		const int destIndex = destBuffer.length();
		destBuffer.copy(destIndex, source, length);
		destBuffer[destIndex + length] = '\0';
	}
	template<int Length>
	void strcat_s(MyString<Length>& destBuffer, const char** sourceArray, const int sourceCount)
	{
		int destIndex = destBuffer.length();
		FOR_I(sourceCount)
		{
			const char* curSource = sourceArray[i];
			if (curSource == nullptr)
			{
				continue;
			}
			const int length = strlength(curSource);
			destBuffer.copy(destIndex, curSource, length);
			destIndex += length;
		}
		destBuffer[destIndex] = '\0';
	}
	template<int SourceLength>
	void strcat_s(char* destBuffer, const int destSize, const Array<SourceLength, const char*>& sourceArray)
	{
		int destIndex = strlength(destBuffer, destSize);
		FOR_I(SourceLength)
		{
			const char* curSource = sourceArray[i];
			if (curSource == nullptr)
			{
				continue;
			}
			const int length = strlength(curSource);
			if (destIndex + length >= destSize)
			{
				ERROR("strcat_s buffer is too small");
				break;
			}
			MEMCPY(destBuffer + destIndex, destSize - destIndex, curSource, length);
			destIndex += length;
		}
		destBuffer[destIndex] = '\0';
	}
	template<typename... TypeList>
	void strcat_t(char* destBuffer, const int destSize, TypeList&&... params)
	{
		strcat_s(destBuffer, destSize, Array<sizeof...(params), const char*>{ forward<TypeList>(params)... });
	}
	template<int Length, int SourceLength>
	void strcat_s(MyString<Length>& destBuffer, const Array<SourceLength, const char*>& sourceArray)
	{
		int destIndex = destBuffer.length();
		FOR_I(SourceLength)
		{
			const char* curSource = sourceArray[i];
			if (curSource == nullptr)
			{
				continue;
			}
			const int length = strlength(curSource);
			destBuffer.copy(destIndex, curSource, length);
			destIndex += length;
		}
		destBuffer[destIndex] = '\0';
	}
	template<int Length, typename... TypeList>
	void strcat_t(MyString<Length>& destBuffer, TypeList&&... params)
	{
		strcat_s(destBuffer, Array<sizeof...(params), const char*>{ forward<TypeList>(params)... });
	}
	MICRO_LEGEND_FRAME_API void strcpy_s(char* destBuffer, int size, const char* source);
	// 以string类型返回count个0
	MICRO_LEGEND_FRAME_API string zeroString(int zeroCount);
	template<int Length>
	void zeroString(MyString<Length>& charArray, const int zeroCount)
	{
		if ((int)Length <= zeroCount)
		{
			ERROR("buffer is too small");
			return;
		}
		FOR_I(zeroCount)
		{
			charArray[i] = '0';
		}
		charArray[zeroCount] = '\0';
	}
	// 基础数据类型转换为字符串
	//-----------------------------------------------------------------------------------------------------------------------------
	string BToS(const bool value) { return value ? "True" : "False"; }
	// 返回string类型的数字字符串,速度较慢
	MICRO_LEGEND_FRAME_API string LLToS(llong value, int limitLen = 0);
	template<int Length>
	int _i64toa_s(llong value, MyString<Length>& charArray)
	{
		if (value == 0)
		{
			charArray[0] = '0';
			charArray[1] = '\0';
			return 1;
		}
		int sign = 1;
		// 负数需要转换为正数才能正常转换为字符串
		if (value < 0)
		{
			value = -value;
			sign = -1;
		}
		const llong maxLLong = POWER_LLONG_10[POWER_LLONG_10.size() - 1] - 1;
		if (value > maxLLong)
		{
			value = maxLLong;
		}
		int index = 0;
		while (true)
		{
			// 如果是正数,则数字个数不能超过size - 1
			// 如果是负数,则数字个数不能超过size - 2
			if ((sign > 0 && index >= (int)Length) ||
				(sign < 0 && index >= (int)Length - 1))
			{
				ERROR("buffer is too small!");
				break;
			}
			// 将数字放入numberArray的尾部
			if (value < POWER_LLONG_10[index])
			{
				break;
			}
			charArray[Length - 1 - index] = (int)(value % POWER_LLONG_10[index + 1] / POWER_LLONG_10[index]);
			++index;
		}
		// 将数字从数组末尾移动到开头,并且将数字转换为数字字符
		if (sign > 0)
		{
			const int lengthToHead = Length - index;
			FOR_I(index)
			{
				charArray[i] = charArray[i + lengthToHead] + '0';
			}
			charArray[index] = '\0';
		}
		else
		{
			charArray[0] = '-';
			const int lengthToHead = Length - index;
			FOR_I(index)
			{
				charArray[i + 1] = charArray[i + lengthToHead] + '0';
			}
			charArray[++index] = '\0';
		}
		return index;
	}
	template<int Length>
	int LLToS(MyString<Length>& charArray, const llong value, const int limitLen = 0)
	{
		if (limitLen == 0)
		{
			return _i64toa_s(value, charArray);
		}
		MyString<32> temp;
		const int len = _i64toa_s(value, temp);
		// 判断是否需要在前面补0
		if (limitLen > len)
		{
			MyString<16> zeroArray;
			zeroString(zeroArray, limitLen - len);
			strcat_t(charArray, zeroArray.str(), temp.str());
			charArray[limitLen] = '\0';
			return limitLen;
		}
		else
		{
			charArray.copy(temp.str(), len);
			charArray[len] = '\0';
			return len;
		}
	}
	template<int Length>
	int _itoa_s(int value, MyString<Length>& charArray)
	{
		if (value == 0)
		{
			charArray[0] = '0';
			charArray[1] = '\0';
			return 1;
		}

		if (mIntString[0].length() == 0)
		{
			initIntToString();
		}
		// 优先查表,但是前提是表已经有值
		if (mIntString[mIntString.size() - 1].length() > 0 && value >= 0 && value < (int)mIntString.size())
		{
			const string& str = mIntString[value];
			const int strLength = (int)str.length();
			if (strLength + 1 >= (int)Length)
			{
				ERROR("int value is too large:" + LLToS((llong)value));
				return 0;
			}
			charArray.copy(str, strLength);
			charArray[strLength] = '\0';
			return strLength;
		}

		int sign = 1;
		if (value < 0)
		{
			value = -value;
			sign = -1;
		}
		if ((llong)value > POWER_INT_10[POWER_INT_10.size() - 1])
		{
			ERROR("int value is too large:" + LLToS((llong)value));
			return 0;
		}
		int index = 0;
		while (true)
		{
			// 如果是正数,则数字个数不能超过size - 1
			// 如果是负数,则数字个数不能超过size - 2
			if ((sign > 0 && index >= (int)Length) ||
				(sign < 0 && index >= (int)Length - 1))
			{
				ERROR("buffer is too small!");
				break;
			}
			// 将数字放入numberArray的尾部
			if ((llong)value < POWER_INT_10[index])
			{
				break;
			}
			charArray[Length - 1 - index] = (int)((llong)value % POWER_INT_10[index + 1] / POWER_INT_10[index]);
			++index;
		}
		// 将数字从数组末尾移动到开头,并且将数字转换为数字字符
		if (sign > 0)
		{
			const int lengthToHead = Length - index;
			FOR_I(index)
			{
				charArray[i] = charArray[i + lengthToHead] + '0';
			}
			charArray[index] = '\0';
		}
		else
		{
			charArray[0] = '-';
			const int lengthToHead = Length - index;
			FOR_I(index)
			{
				charArray[i + 1] = charArray[i + lengthToHead] + '0';
			}
			charArray[++index] = '\0';
		}
		return index;
	}
	// 这里有问题,应该是
	template<int Length>
	int _uitoa_s(const uint value, MyString<Length>& charArray)
	{
		if (value == 0)
		{
			charArray[0] = '0';
			charArray[1] = '\0';
			return 1;
		}

		if (mIntString[0].length() == 0)
		{
			initIntToString();
		}
		// 优先查表,但是前提是表已经有值
		if (mIntString[mIntString.size() - 1].length() > 0 && value < (uint)mIntString.size())
		{
			const string& str = mIntString[value];
			const int strLength = (int)str.length();
			if (strLength + 1 >= (int)Length)
			{
				ERROR("uint value is too large:" + LLToS((llong)value));
				return 0;
			}
			charArray.copy(str, strLength);
			charArray[strLength] = '\0';
			return strLength;
		}

		if ((llong)value > POWER_INT_10[POWER_INT_10.size() - 1])
		{
			ERROR("uint value is too large:" + LLToS((llong)value));
			return 0;
		}
		int index = 0;
		while (true)
		{
			// 数字个数不能超过size - 1
			if (index >= (int)Length)
			{
				ERROR("buffer is too small!");
				break;
			}
			// 将数字放入numberArray的尾部
			if ((llong)value < POWER_INT_10[index])
			{
				break;
			}
			charArray[Length - 1 - index] = (int)((llong)value % POWER_INT_10[index + 1] / POWER_INT_10[index]);
			++index;
		}
		// 将数字从数组末尾移动到开头,并且将数字转换为数字字符
		const int lengthToHead = Length - index;
		FOR_I(index)
		{
			charArray[i] = charArray[i + lengthToHead] + '0';
		}
		charArray[index] = '\0';
		return index;
	}
	template<int Length>
	int _ui64toa_s(const ullong value, MyString<Length>& charArray)
	{
		if (value == 0)
		{
			charArray[0] = '0';
			charArray[1] = '\0';
			return 1;
		}
		if (value > (ullong)POWER_LLONG_10[POWER_LLONG_10.size() - 1])
		{
			ERROR("ullong value is too large");
			return 0;
		}
		int index = 0;
		while (true)
		{
			// 如果是正数,则数字个数不能超过Length - 1
			if (index >= (int)Length)
			{
				ERROR("buffer is too small!");
				break;
			}
			// 将数字放入numberArray的尾部
			if (value < (ullong)POWER_LLONG_10[index])
			{
				break;
			}
			charArray[Length - 1 - index] = (int)(value % POWER_LLONG_10[index + 1] / POWER_LLONG_10[index]);
			++index;
		}
		// 将数字从数组末尾移动到开头,并且将数字转换为数字字符
		const int lengthToHead = Length - index;
		FOR_I(index)
		{
			charArray[i] = charArray[i + lengthToHead] + '0';
		}
		charArray[index] = '\0';
		return index;
	}
	// 返回string类型的数字字符串,速度较慢,limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	MICRO_LEGEND_FRAME_API string UIToS(int value, int limitLen = 0);
	template<int Length>
	int UIToS(MyString<Length>& charArray, const int value, const int limitLen = 0)
	{
		if (limitLen == 0)
		{
			return _itoa_s(value, charArray);
		}
		// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
		MyString<16> temp;
		const int len = _itoa_s(value, temp);
		// 判断是否需要在前面补0
		if (limitLen > len)
		{
			// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
			MyString<16> zeroArray;
			zeroString(zeroArray, limitLen - len);
			strcat_t(charArray, zeroArray.str(), temp.str());
			charArray[limitLen] = '\0';
			return limitLen;
		}
		else
		{
			charArray.copy(temp.str(), len);
			charArray[len] = '\0';
			return len;
		}
	}
	// 返回string类型的数字字符串,速度较慢,limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	MICRO_LEGEND_FRAME_API string IToS(const int value, int limitLen = 0);
	template<int Length>
	int IToS(MyString<Length>& charArray, const int value, const int limitLen = 0)
	{
		if (limitLen == 0)
		{
			return _itoa_s(value, charArray);
		}
		// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
		MyString<16> temp;
		const int len = _itoa_s(value, temp);
		// 判断是否需要在前面补0
		if (limitLen > len)
		{
			// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
			MyString<16> zeroArray;
			zeroString(zeroArray, limitLen - len);
			strcat_t(charArray, zeroArray.str(), temp.str());
			charArray[limitLen] = '\0';
			return limitLen;
		}
		else
		{
			charArray.copy(temp.str(), len);
			charArray[len] = '\0';
			return len;
		}
	}
	// 返回string类型的数字字符串,速度较慢
	MICRO_LEGEND_FRAME_API string ULLToS(ullong value, int limitLen = 0);
	template<int Length>
	int ULLToS(MyString<Length>& charArray, const ullong value, const int limitLen = 0)
	{
		if (limitLen == 0)
		{
			return _ui64toa_s(value, charArray);
		}
		MyString<32> temp;
		const int len = _ui64toa_s(value, temp);
		// 判断是否需要在前面补0
		if (limitLen > len)
		{
			MyString<16> zeroArray;
			zeroString(zeroArray, limitLen - len);
			strcat_t(charArray, zeroArray.str(), temp.str());
			charArray[limitLen] = '\0';
			return limitLen;
		}
		else
		{
			charArray.copy(temp.str(), len);
			charArray[len] = '\0';
			return len;
		}
	}
	// precision为精度,保留的小数的位数,removeZero为是否去除末尾无用的0,速度较慢
	MICRO_LEGEND_FRAME_API string floatToStringExtra(float f, int precision = 4, bool removeTailZero = true);
	// 将浮点数转换为字符串,速度较快
	MICRO_LEGEND_FRAME_API string FToS(float f);
	template<int Length>
	int FToS(MyString<Length>& charArray, float f)
	{
		if (f > 99999999.0f)
		{
			f = 99999999.0f;
		}
		else if (f < -99999999.0f)
		{
			f = -99999999.0f;
		}
		SPRINTF(charArray.toBuffer(), Length, "%.4f", f);
		charArray[Length - 1] = '\0';
		// 先查找小数点和结束符的位置
		int dotPos = -1;
		int strLen = 0;
		FOR_I(Length)
		{
			if (charArray[i] == '.')
			{
				dotPos = i;
			}
			else if (charArray[i] == '\0')
			{
				strLen = i;
				break;
			}
		}
		if (dotPos >= 0)
		{
			// 从结束符往前查找
			FOR_I(strLen)
			{
				const int index = strLen - 1 - i;
				// 如果找到了小数点仍然没有找到一个不为'0'的字符,则从小数点部分截断字符串
				if (index == dotPos)
				{
					charArray[dotPos] = '\0';
					strLen = dotPos;
					break;
				}
				// 找到小数点后的从后往前的第一个不为'0'的字符,从此处截断字符串
				if (charArray[index] != '0' && index + 1 < strLen)
				{
					charArray[index + 1] = '\0';
					strLen = index + 1;
					break;
				}
			}
		}
		return strLen;
	}
	MICRO_LEGEND_FRAME_API string bytesToString(const char* buffer, int length);
	string V2ToS(const Vector2& vec, const char* seperate = ",") { return FToS(vec.x) + seperate + FToS(vec.y); }
	template<int Length>
	void V2ToS(MyString<Length>& buffer, const Vector2& vec, const char* seperate = ",")
	{
		buffer[0] = '\0';
		FLOAT_STR(xStr, vec.x);
		FLOAT_STR(yStr, vec.y);
		strcat_t(buffer, xStr.str(), seperate, yStr.str());
	}
	string V3ToS(const Vector3& vec, const char* seperate = ",") { return FToS(vec.x) + seperate + FToS(vec.y) + seperate + FToS(vec.z); }
	template<int Length>
	void V3ToS(MyString<Length>& buffer, const Vector3& vec, const char* seperate = ",")
	{
		buffer[0] = '\0';
		FLOAT_STR(xStr, vec.x);
		FLOAT_STR(yStr, vec.y);
		FLOAT_STR(zStr, vec.z);
		strcat_t(buffer, xStr.str(), seperate, yStr.str(), seperate, zStr.str());
	}
	string V3IToS(const Vector3Int& vec, const char* seperate = ",") { return IToS(vec.x) + seperate + IToS(vec.y) + seperate + IToS(vec.z); }
	template<int Length>
	void V3IToS(MyString<Length>& buffer, const Vector3Int& vec, const char* seperate = ",")
	{
		buffer[0] = '\0';
		INT_STR(xStr, vec.x);
		INT_STR(yStr, vec.y);
		INT_STR(zStr, vec.z);
		strcat_t(buffer, xStr.str(), seperate, yStr.str(), seperate, zStr.str());
	}
	string V2IToS(const Vector2Int& value, const char* seperate = ",") { return IToS(value.x) + seperate + IToS(value.y); }
	string V2UIToS(const Vector2UInt& value, const char* seperate = ",") { return UIToS(value.x) + seperate + UIToS(value.y); }
	template<int Length>
	void V2IToS(MyString<Length>& buffer, const Vector2Int& value, const char* seperate = ",")
	{
		buffer[0] = '\0';
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		strcat_t(buffer, xStr.str(), seperate, yStr.str());
	}
	template<int Length>
	void V2UIToS(MyString<Length>& buffer, const Vector2UInt& value, const char* seperate = ",")
	{
		buffer[0] = '\0';
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		strcat_t(buffer, xStr.str(), seperate, yStr.str());
	}
	string V2SToS(const Vector2Short& value, const char* seperate = ",") { return IToS(value.x) + seperate + IToS(value.y); }
	template<int Length>
	void V2SToS(MyString<Length>& buffer, const Vector2Short& value, const char* seperate = ",")
	{
		buffer[0] = '\0';
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		strcat_t(buffer, xStr.str(), seperate, yStr.str());
	}
	string V2USToS(const Vector2UShort& value, const char* seperate = ",") { return IToS(value.x) + seperate + IToS(value.y); }
	template<int Length>
	void V2USToS(MyString<Length>& buffer, const Vector2UShort& value, const char* seperate = ",")
	{
		buffer[0] = '\0';
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		strcat_t(buffer, xStr.str(), seperate, yStr.str());
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	// 字符串转换为基础数据类型
	//-----------------------------------------------------------------------------------------------------------------------------
	bool SToB(const string& str) { return str == "True" || str == "true"; }
	bool SToB(const char* str) { return strcmp(str, "True") == 0 || strcmp(str, "true") == 0; }
	int SToI(const string& str) { return atoi(str.c_str()); }
	int SToI(const char* str) { return atoi(str); }
	ullong stringToULLong(const string& str) { return (ullong)atoll(str.c_str()); }
	ullong stringToULLong(const char* str) { return (ullong)atoll(str); }
	llong SToLL(const string& str) { return atoll(str.c_str()); }
	llong SToLL(const char* str) { return atoll(str); }
	float SToF(const string& str) { return (float)atof(str.c_str()); }
	float SToF(const char* str) { return (float)atof(str); }
	MICRO_LEGEND_FRAME_API Vector2 SToV2(const string& str, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API Vector2Int SToV2I(const string& str, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API Vector2UInt SToV2UI(const string& str, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API Vector2Short SToV2S(const string& str, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API Vector2UShort SToV2US(const string& str, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API Vector3 SToV3(const string& str, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API Vector3Int SToV3I(const string& str, const char* seperate = ",");
	//-----------------------------------------------------------------------------------------------------------------------------
	// 基础数据类型数组转换为字符串
	//-----------------------------------------------------------------------------------------------------------------------------
	MICRO_LEGEND_FRAME_API string ULLsToS(const Vector<ullong>& valueList, const char* seperate = ",");
	template<int Length>
	string ULLsToS(const Array<Length, ullong>& valueList, const int count = -1, const char* seperate = ",")
	{
		if (count == -1)
		{
			count = valueList.size();
		}
		// 根据列表长度选择适当的数组长度,每个llong默认数字长度为32个字符
		const int arrayLen = 32 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		MyString<32> temp;
		FOR_I(count)
		{
			ULLToS(temp, valueList[i]);
			if (i != count - 1)
			{
				strcat_t(charArray.mArray, arrayLen, temp.str(), seperate);
			}
			else
			{
				strcat_s(charArray.mArray, arrayLen, temp.str());
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void ULLsToS(MyString<Length>& buffer, const ullong* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}
		MyString<32> temp;
		FOR_I(count)
		{
			ULLToS(temp, valueList[i]);
			if (i != count - 1)
			{
				strcat_t(buffer, temp.str(), seperate);
			}
			else
			{
				strcat_s(buffer, temp.str());
			}
		}
	}
	template<int Length>
	void ULLsToS(MyString<Length>& buffer, const Vector<ullong>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}
		MyString<32> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			ULLToS(temp, valueList[i]);
			if (i != count - 1)
			{
				strcat_t(buffer, temp.str(), seperate);
			}
			else
			{
				strcat_s(buffer, temp.str());
			}
		}
	}
	void ULLsToS(char* buffer, const int bufferSize, const Vector<ullong>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}
		MyString<32> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			ULLToS(temp, valueList[i]);
			if (i != count - 1)
			{
				strcat_t(buffer, bufferSize, temp.str(), seperate);
			}
			else
			{
				strcat_s(buffer, bufferSize, temp.str());
			}
		}
	}
	MICRO_LEGEND_FRAME_API string LLsToS(const Vector<llong>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API string LLsToS(const llong* valueList, int valueCount, const char* seperate = ",");
	template<int Length>
	string LLsToS(const Array<Length, llong>& valueList, int count = -1, const char* seperate = ",")
	{
		if (count == -1)
		{
			count = (int)Length;
		}
		// 根据列表长度选择适当的数组长度,每个llong默认数字长度为32个字符
		const int arrayLen = 32 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int seperateLen = strlength(seperate);
		MyString<32> temp;
		FOR_I(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, seperate, seperateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	string LLsToS(const ArrayList<Length, llong>& valueList, const char* seperate = ",")
	{
		const int count = valueList.size();
		// 根据列表长度选择适当的数组长度,每个llong默认数字长度为32个字符
		const int arrayLen = 32 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int seperateLen = strlength(seperate);
		MyString<32> temp;
		FOR_I(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, seperate, seperateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void LLsToS(MyString<Length>& buffer, const llong* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}
		const int seperateLen = strlength(seperate);
		MyString<32> temp;
		FOR_I(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void LLsToS(MyString<Length>& buffer, const Vector<llong>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}
		const int seperateLen = strlength(seperate);
		MyString<32> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	void LLsToS(char* buffer, const int bufferSize, const Vector<llong>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}
		const int seperateLen = strlength(seperate);
		MyString<32> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, seperate, seperateLen);
			}
		}
	}
	template<int Length, int ValueCount>
	void LLsToS(MyString<Length>& buffer, const Array<ValueCount, llong>& valueList, int count = -1, const char* seperate = ",")
	{
		if (count == -1)
		{
			count = (int)ValueCount;
		}
		const int seperateLen = strlength(seperate);
		buffer[0] = '\0';
		MyString<32> temp;
		FOR_I(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	// 将byte数组当成整数数组转换为字符串,并非直接将byte数组转为字符串
	MICRO_LEGEND_FRAME_API string bytesToString(const Vector<byte>& valueList, const char* seperate = ",");
	template<int Length>
	void bytesToString(MyString<Length>& buffer, byte* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<4> temp;
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void bytesToString(MyString<Length>& buffer, const Vector<byte>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<4> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	void bytesToString(char* buffer, const int bufferSize, const Vector<byte>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<4> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, seperate, seperateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API string SsToS(const Vector<short>& valueList, const char* seperate = ",");
	template<int Length>
	void SsToS(MyString<Length>& buffer, short* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<8> temp;
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void SsToS(MyString<Length>& buffer, const Vector<short>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	void SsToS(char* buffer, const int bufferSize, const Vector<short>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, seperate, seperateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API string USsToS(const Vector<ushort>& valueList, const char* seperate = ",");
	template<int Length>
	string USsToS(const ArrayList<Length, ushort>& valueList, const char* seperate = ",")
	{
		if (valueList.size() == 0)
		{
			return FrameDefine::EMPTY;
		}
		const int arrayLen = 8 * greaterPower2(valueList.size());
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = '\0';
		const int seperateLen = strlength(seperate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, seperate, seperateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void USsToS(MyString<Length>& buffer, ushort* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<8> temp;
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void USsToS(MyString<Length>& buffer, const Vector<ushort>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	void USsToS(char* buffer, const int bufferSize, const Vector<ushort>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, seperate, seperateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API string IsToS(const Vector<int>& valueList, const char* seperate = ",");
	template<int Length>
	string IsToS(const Array<Length, int>& valueList, const int count, const char* seperate = ",")
	{
		if (count == 0)
		{
			return "";
		}
		// 根据列表长度选择适当的数组长度,每个int默认数字长度为16个字符
		const int arrayLen = 16 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, seperate, seperateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	string IsToS(const Array<Length, int>& valueList, const char* seperate = ",")
	{
		const int count = valueList.size();
		if (count == 0)
		{
			return "";
		}
		// 根据列表长度选择适当的数组长度,每个int默认数字长度为16个字符
		const int arrayLen = 16 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, seperate, seperateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	string IsToS(const ArrayList<Length, int>& valueList, const char* seperate = ",")
	{
		const int count = valueList.size();
		if (count == 0)
		{
			return "";
		}
		// 根据列表长度选择适当的数组长度,每个int默认数字长度为16个字符
		const int arrayLen = 16 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, seperate, seperateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void IsToS(MyString<Length>& buffer, int* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void IsToS(MyString<Length>& buffer, const Vector<int>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	void IsToS(char* buffer, int bufferSize, const Vector<int>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void IsToS(char* buffer, int bufferSize, const ArrayList<Length, int>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, seperate, seperateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API string UIsToS(const Vector<uint>& valueList, const char* seperate = ",");
	template<int Length>
	void UIsToS(MyString<Length>& buffer, uint* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		FOR_I(count)
		{
			const int len = UIToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void UIsToS(MyString<Length>& buffer, const Vector<uint>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = UIToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	void UIsToS(char* buffer, int bufferSize, const Vector<uint>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = UIToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, seperate, seperateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API string FsToS(const Vector<float>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API void FsToS(char* buffer, int bufferSize, const Vector<float>& valueList, const char* seperate = ",");
	template<int Length>
	void FsToS(MyString<Length>& buffer, float* valueList, const int count, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		FOR_I(count)
		{
			const int len = FToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void FsToS(MyString<Length>& buffer, const Vector<float>& valueList, const char* seperate = ",")
	{
		buffer[0] = '\0';
		if (valueList.size() == 0)
		{
			return;
		}

		const int seperateLen = strlength(seperate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR_I(count)
		{
			const int len = FToS(temp, valueList[i]);
			strcat_s(buffer, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	template<int Length>
	void stringsToString(MyString<Length>& buffer, const char** strList, const int stringCount, const char* seperate = ",")
	{
		const int seperateLen = strlength(seperate);
		buffer[0] = '\0';
		FOR_I(stringCount)
		{
			strcat_s(buffer, strList[i]);
			if (i != stringCount - 1)
			{
				strcat_s(buffer, seperate, seperateLen);
			}
		}
	}
	string stringsToString(const Vector<string>& strList, const char* seperate = ",")
	{
		string totalStr;
		FOR_VECTOR(strList)
		{
			totalStr += strList[i];
			if (i != strList.size() - 1)
			{
				totalStr += seperate;
			}
		}
		return totalStr;
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	// 字符串转换为基础数据类型数组
	//-----------------------------------------------------------------------------------------------------------------------------
	MICRO_LEGEND_FRAME_API void SToBs(const string& str, Vector<byte>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API int SToBs(const char* str, byte* buffer, int bufferSize, const char* seperate = ",");
	template<int Length>
	int SToBs(const char* str, Array<Length, byte>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int keyLen = strlength(seperate);
		const int sourceLen = strlength(str);
		MyString<4> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("int buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToI(curString.str());
		}
		return curCount;
	}
	MICRO_LEGEND_FRAME_API void SToSs(const string& str, Vector<short>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API int SToSs(const char* str, short* buffer, int bufferSize, const char* seperate = ",");
	template<int Length>
	int SToSs(const string& str, Array<Length, short>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(seperate);
		MyString<16> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str, startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("int buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToI(curString.str());
		}
		return curCount;
	}
	MICRO_LEGEND_FRAME_API void SToUSs(const string& str, Vector<ushort>& valueList, const char* seperate = ",");
	template<int Length>
	void SToUSs(const string& str, ArrayList<Length, ushort>& valueList, const char* seperate = ",")
	{
		Vector<string> strList;
		split(str, seperate, strList);
		valueList.clear();
		FOR_I(strList.size())
		{
			const string& curStr = strList[i];
			if (curStr.length() > 0)
			{
				valueList.add(SToI(curStr));
			}
		}
	}
	MICRO_LEGEND_FRAME_API int SToUSs(const char* str, ushort* buffer, int bufferSize, const char* seperate = ",");
	template<int Length>
	int SToUSs(const char* str, Array<Length, ushort>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(seperate);
		MyString<8> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("int buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToI(curString.str());
		}
		return curCount;
	}
	MICRO_LEGEND_FRAME_API void SToIs(const string& str, Vector<int>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API int SToIs(const char* str, int* buffer, int bufferSize, const char* seperate = ",");
	template<int Length>
	int SToIs(const string& str, Array<Length, int>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(seperate);
		MyString<16> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str, startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("int buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToI(curString.str());
		}
		return curCount;
	}
	template<int Length>
	void SToIs(const string& str, ArrayList<Length, int>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(seperate);
		MyString<16> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str, startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (!buffer.add(SToI(curString.str())))
			{
				if (showError)
				{
					ERROR("int buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
		}
	}
	template<int Length>
	int SToIs(const char* str, Array<Length, int>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(seperate);
		MyString<16> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("int buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToI(curString.str());
		}
		return curCount;
	}
	MICRO_LEGEND_FRAME_API void SToUIs(const string& str, Vector<uint>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API int SToUIs(const char* str, uint* buffer, int bufferSize, const char* seperate = ",");
	template<int Length>
	int SToUIs(const char* str, Array<Length, uint>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(seperate);
		MyString<16> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为长整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("uint buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToI(curString.str());
		}
		return curCount;
	}
	MICRO_LEGEND_FRAME_API void stringToULLongs(const char* str, Vector<ullong>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API int stringToULLongs(const char* str, ullong* buffer, int bufferSize, const char* seperate = ",");
	template<int Length>
	int stringToULLongs(const char* str, Array<Length, ullong>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(seperate);
		MyString<32> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为长整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("ullong buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = stringToULLong(curString.str());
		}
		return curCount;
	}
	MICRO_LEGEND_FRAME_API void SToLLs(const char* str, Vector<llong>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API void SToLLs(const string& str, Vector<llong>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API Vector<llong> SToLLs(const string& str, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API int SToLLs(const char* str, llong* buffer, int bufferSize, const char* seperate = ",");
	template<int Length>
	int SToLLs(const string& str, Array<Length, llong>& buffer, int destOffset = 0, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = destOffset;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(seperate);
		MyString<32> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str, startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为长整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					LOG("llong buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToLL(curString.str());
		}
		return curCount;
	}
	template<int Length>
	void SToLLs(const string& str, ArrayList<Length, llong>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(seperate);
		MyString<32> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str, startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为长整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (!buffer.add(SToLL(curString.str())))
			{
				if (showError)
				{
					LOG("llong buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
		}
	}
	template<int Length>
	int SToLLs(const char* str, Array<Length, llong>& buffer, int destOffset = 0, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = destOffset;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(seperate);
		MyString<32> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为长整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("llong buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToLL(curString.str());
		}
		return curCount;
	}
	template<int Length>
	void SToLLs(const char* str, ArrayList<Length, llong>& buffer, int destOffset = 0, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(seperate);
		MyString<32> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为长整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (!buffer.add(SToLL(curString.str())))
			{
				if (showError)
				{
					ERROR("llong buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
		}
	}
	MICRO_LEGEND_FRAME_API void SToFs(const string& str, Vector<float>& valueList, const char* seperate = ",");
	template<int Length>
	int SToFs(const char* str, Array<Length, float>& buffer, const char* seperate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(seperate);
		MyString<32> curString;
		int devidePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, seperate, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 转换为长整数放入列表
			if (curString[0] == '\0')
			{
				continue;
			}
			if (curCount >= (int)Length)
			{
				if (showError)
				{
					ERROR("float buffer size is too small, bufferSize:" + IToS(Length));
				}
				break;
			}
			buffer[curCount++] = SToF(curString.str());
		}
		return curCount;
	}
	MICRO_LEGEND_FRAME_API void SToV2Is(const string& str, Vector<Vector2Int>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API void SToV2s(const string& str, Vector<Vector2>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API void SToV3Is(const string& str, Vector<Vector3Int>& valueList, const char* seperate = ",");
	MICRO_LEGEND_FRAME_API void SToV3s(const string& str, Vector<Vector3>& valueList, const char* seperate = ",");
	// 从一个ullong数组的字符串中移除指定的value的字符串
	template<int Length>
	bool removeULLongsString(MyString<Length>& valueArrayString, const ullong value)
	{
		ULLONG_STR(valueString, value);
		// 如果value是在最后,则只移除value字符串
		if (endWith(valueArrayString.str(), valueString.str()))
		{
			return removeString(valueArrayString, valueString.str());
		}
		// value不在最后,则移除value字符串加后面的逗号
		else
		{
			MyString<32> needRemoveString;
			strcat_t(needRemoveString, valueString.str(), ",");
			return removeString(valueArrayString, needRemoveString.str());
		}
	}
	// 将value添加到一个ullong数组的字符串中
	template<int Length>
	void addULLongsString(MyString<Length>& valueArrayString, const ullong value)
	{
		ULLONG_STR(idStr, value);
		if (valueArrayString[0] != '\0')
		{
			strcat_t(valueArrayString, ",", idStr.str());
		}
		else
		{
			valueArrayString.copy(idStr);
		}
	}
	// 从一个llong数组的字符串中移除指定的value的字符串
	template<int Length>
	bool removeLLongsString(MyString<Length>& valueArrayString, const llong value)
	{
		LLONG_STR(valueString, value);
		// 如果value是在最后,则只移除value字符串
		if (endWith(valueArrayString.str(), valueString.str()))
		{
			return removeString(valueArrayString, valueString.str());
		}
		// value不在最后,则移除value字符串加后面的逗号
		else
		{
			MyString<32> needRemoveString;
			strcat_t(needRemoveString, valueString.str(), ",");
			return removeString(valueArrayString, needRemoveString.str());
		}
	}
	// 将value添加到一个llong数组的字符串中
	template<int Length>
	void addLLongsString(MyString<Length>& valueArrayString, const llong value)
	{
		LLONG_STR(idStr, value);
		if (valueArrayString[0] != '\0')
		{
			strcat_t(valueArrayString, ",", idStr.str());
		}
		else
		{
			valueArrayString.copy(idStr);
		}
	}
	template<int Length>
	void split(const string& str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(key);
		constexpr int STRING_BUFFER = 1024;
		MyString<STRING_BUFFER> curString;
		int devidePos = -1;
		int startPos = 0;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, key, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			if (devidePos - startPos >= STRING_BUFFER)
			{
				ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + IToS(STRING_BUFFER) + "字节");
				return;
			}
			curString.copy(str, startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 放入列表
			if (curString[0] == '\0' || !removeEmpty)
			{
				continue;
			}
			if (!stringBuffer.add(curString.str()))
			{
				if (showError)
				{
					ERROR("string buffer is too small! bufferSize:" + IToS(Length));
				}
				break;
			}
		}
	}
	template<int Length>
	void split(const char* str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		const int sourceLen = strlength(str);
		const int keyLen = strlength(key);
		constexpr int STRING_BUFFER = 1024;
		MyString<STRING_BUFFER> curString;
		int devidePos = -1;
		int startPos = 0;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, key, &devidePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			devidePos = ret ? devidePos : sourceLen;
			if (devidePos - startPos >= STRING_BUFFER)
			{
				ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + IToS(STRING_BUFFER) + "字节");
				return;
			}
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + keyLen;
			// 放入列表
			if (curString[0] == '\0' || !removeEmpty)
			{
				continue;
			}
			if (!stringBuffer.add(curString.str()))
			{
				if (showError)
				{
					ERROR("string buffer is too small! bufferSize:" + IToS(Length));
				}
				break;
			}
		}
	}
	template<int Length>
	void split(const char* str, const char key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		int startPos = 0;
		const int sourceLen = strlength(str);
		constexpr int STRING_BUFFER = 1024;
		MyString<STRING_BUFFER> curString;
		bool ret = true;
		while (ret)
		{
			int devidePos = strchar(str, key, startPos, sourceLen);
			// 无论是否查找到,都将前面一段字符串截取出来
			if (devidePos == -1)
			{
				devidePos = sourceLen;
				ret = false;
			}
			if (devidePos - startPos >= STRING_BUFFER)
			{
				ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + IToS(STRING_BUFFER) + "字节");
				return;
			}
			curString.copy(str + startPos, devidePos - startPos);
			curString[devidePos - startPos] = '\0';
			startPos = devidePos + 1;
			// 放入列表
			if (curString[0] != '\0' || !removeEmpty)
			{
				if (!stringBuffer.add(curString.str()))
				{
					if (showError)
					{
						ERROR("string buffer is too small! bufferSize:" + IToS(Length));
					}
					break;
				}
			}
		}
	}

	template<int Length>
	bool splitFull(const char* str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		split(str, key, stringBuffer, removeEmpty, showError);
		return stringBuffer.size() == stringBuffer.maxSize();
	}
	template<int Length>
	bool splitFull(const string& str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		split(str, key, stringBuffer, removeEmpty, showError);
		return stringBuffer.size() == stringBuffer.maxSize();
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	// 移除字符串首部的数字
	MICRO_LEGEND_FRAME_API string removePreNumber(const string& str);
	MICRO_LEGEND_FRAME_API wstring ANSIToUnicode(const char* str);
	MICRO_LEGEND_FRAME_API void ANSIToUnicode(const char* str, wchar_t* output, int maxLength);
	MICRO_LEGEND_FRAME_API void ANSIToUnicode(const string& str, wchar_t* output, int maxLength);
	MICRO_LEGEND_FRAME_API string UnicodeToANSI(const wchar_t* str);
	MICRO_LEGEND_FRAME_API void UnicodeToANSI(const wchar_t* str, char* output, int maxLength);
	MICRO_LEGEND_FRAME_API string UnicodeToUTF8(const wchar_t* str);
	MICRO_LEGEND_FRAME_API void UnicodeToUTF8(const wchar_t* str, char* output, int maxLength);
	MICRO_LEGEND_FRAME_API wstring UTF8ToUnicode(const char* str);
	MICRO_LEGEND_FRAME_API void UTF8ToUnicode(const char* str, wchar_t* output, int maxLength);
	MICRO_LEGEND_FRAME_API void UTF8ToUnicode(const string& str, wchar_t* output, int maxLength);
	MICRO_LEGEND_FRAME_API string ANSIToUTF8(const char* str, bool addBOM = false);
	MICRO_LEGEND_FRAME_API string ANSIToUTF8(const string& str, bool addBOM = false);
	MICRO_LEGEND_FRAME_API void ANSIToUTF8(const char* str, char* output, int maxLength, bool addBOM = false);
	MICRO_LEGEND_FRAME_API void ANSIToUTF8(const string& str, char* output, int maxLength, bool addBOM = false);
	// 根据不同的平台选择不同的实现方式,windows下进行转换,linux直接返回原字符串
#ifdef WINDOWS
	string ANSIToUTF8Auto(const char* str, bool addBOM = false) { return ANSIToUTF8(str, addBOM); }
	string ANSIToUTF8Auto(const string& str, bool addBOM = false) { return ANSIToUTF8(str, addBOM); }
#elif defined LINUX
	string ANSIToUTF8Auto(const char* str, bool addBOM = false) { return str; }
	const string& ANSIToUTF8Auto(const string& str, bool addBOM = false) { return str; }
#endif
	MICRO_LEGEND_FRAME_API string UTF8ToANSI(const char* str, bool eraseBOM = false);
	MICRO_LEGEND_FRAME_API string UTF8ToANSI(const string& str, bool eraseBOM = false);
	MICRO_LEGEND_FRAME_API void UTF8ToANSI(const char* str, char* output, int maxLength, bool eraseBOM = false);
#ifdef WINDOWS
	string UTF8ToANSIAuto(const char* str, bool eraseBOM = false) { return UTF8ToANSI(str, eraseBOM); }
	string UTF8ToANSIAuto(const string& str, bool eraseBOM = false) { return UTF8ToANSI(str, eraseBOM); }
#elif defined LINUX
	string UTF8ToANSIAuto(const char* str, bool eraseBOM = false) { return str; }
	const string& UTF8ToANSIAuto(const string& str, bool eraseBOM = false) { return str; }
#endif
	MICRO_LEGEND_FRAME_API void removeBOM(string& str);
	MICRO_LEGEND_FRAME_API void removeBOM(char* str, int length = 0);
	// json
	MICRO_LEGEND_FRAME_API void jsonStartArray(string& str, int preTableCount = 0, bool returnLine = false);
	MICRO_LEGEND_FRAME_API void jsonEndArray(string& str, int preTableCount = 0, bool returnLine = false);
	MICRO_LEGEND_FRAME_API void jsonStartStruct(string& str, int preTableCount = 0, bool returnLine = false);
	MICRO_LEGEND_FRAME_API void jsonEndStruct(string& str, int preTableCount = 0, bool returnLine = false);
	MICRO_LEGEND_FRAME_API void jsonAddPair(string& str, const string& name, const string& value, int preTableCount = 0, bool returnLine = false);
	MICRO_LEGEND_FRAME_API void jsonAddObject(string& str, const string& name, const string& value, int preTableCount = 0, bool returnLine = false);
	MICRO_LEGEND_FRAME_API string toLower(const string& str);
	MICRO_LEGEND_FRAME_API string toUpper(const string& str);
	bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }
	bool isLower(char c) { return c >= 'a' && c <= 'z'; }
	bool isNumber(char c) { return c >= '0' && c <= '9'; }
	MICRO_LEGEND_FRAME_API bool isNumber(const char* str, int length = 0);
	bool isLetter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
	char toLower(const char value)
	{
		if (isUpper(value))
		{
			return value + 'a' - 'A';
		}
		return value;
	}
	char toUpper(const char value)
	{
		if (isLower(value))
		{
			return value - ('a' - 'A');
		}
		return value;
	}
	MICRO_LEGEND_FRAME_API void rightToLeft(string& str);
	MICRO_LEGEND_FRAME_API void leftToRight(string& str);
	MICRO_LEGEND_FRAME_API bool checkString(const string& str, const string& valid);
	bool checkIntString(const string& str, const string& valid = "") { return checkString(str, "0123456789" + valid); }
	bool checkFloatString(const string& str, const string& valid = "") { return checkIntString(str, "." + valid); }
	MICRO_LEGEND_FRAME_API bool hasNonAscii(const char* str);
	MICRO_LEGEND_FRAME_API string charToHexString(byte value, bool upper = true);
	MICRO_LEGEND_FRAME_API int getCharCount(const string& str, char key);
	MICRO_LEGEND_FRAME_API int getCharCount(const char* str, char key);
	MICRO_LEGEND_FRAME_API bool isPhoneNumber(const char* str);
	MICRO_LEGEND_FRAME_API string bytesToHexString(const byte* data, int dataCount, bool addSpace = true, bool upper = true);
	byte hexCharToByte(const char hex)
	{
		if (isNumber(hex))
		{
			return hex - '0';
		}
		if (isUpper(hex))
		{
			return 0x0A + hex - 'A';
		}
		return 0;
	}
	char byteToHexChar(const byte value)
	{
		if (value <= 9)
		{
			return value + '0';
		}
		if (value <= 0x0F)
		{
			return value - 0x0A + 'A';
		}
		return 0;
	}
	// 字符串中是否包含控制字符
	MICRO_LEGEND_FRAME_API bool hasControlChar(const char* str);
	// 字符串中是否包含控制字符,英文空格,中文空格
	MICRO_LEGEND_FRAME_API bool hasInvisibleChar(const char* str);
	// sql语法相关字符串处理
	// 带任何引号都不允许
	bool checkSQLString(const char* str) { return strchar(str, '\'') < 0 && strchar(str, '\"') < 0; }
	// 带任何引号都不允许
	bool checkSQLString(const string& str) { return strchar(str.c_str(), '\'', 0, (int)str.length()) < 0 && strchar(str.c_str(), '\"', 0, (int)str.length()) < 0; }
	// insert相关
	template<int Length>
	void sqlAdd(MyString<Length>& sqlStr)
	{
		// 当递归展开到最后一层时移除最后一个逗号,确保不是以逗号结尾
		const int length = sqlStr.length();
		if (length > 0 && sqlStr[length - 1] == ',')
		{
			sqlStr[length - 1] = '\0';
		}
		return;
	}
	MICRO_LEGEND_FRAME_API void sqlAddString(char* queryStr, int size, const char* str, bool addComma);
	MICRO_LEGEND_FRAME_API void sqlAddStringUTF8(char* queryStr, int size, const char* str, bool addComma);
	MICRO_LEGEND_FRAME_API void sqlAddStringUTF8(string& queryStr, const string& str, bool addComma);
	template<int Length>
	void sqlAddString(MyString<Length>& queryStr, const char* str, const bool addComma)
	{
		if (checkSQLString(str))
		{
			strcat_t(queryStr, "\"", str, addComma ? "\"," : "\"");
		}
		else
		{
			strcat_t(queryStr, "\"", addComma ? "\"," : "\"");
		}
	}
	void sqlAddString(string& queryStr, const string& str, const bool addComma)
	{
		if (checkSQLString(str))
		{
			queryStr += "\"";
			queryStr += str;
			queryStr += addComma ? "\"," : "\"";
		}
		else
		{
			queryStr += "\"";
			queryStr += addComma ? "\"," : "\"";
		}
	}
	template<int Length>
	void sqlAddStringUTF8(MyString<Length>& queryStr, const char* str, const bool addComma)
	{
		if (str[0] != '\0')
		{
			char* utf8String = nullptr;
			CharArrayScopeThread scope(utf8String, Length);
			ANSIToUTF8(str, utf8String, Length, false);
			if (checkSQLString(str))
			{
				strcat_t(queryStr, "\"", utf8String, addComma ? "\"," : "\"");
			}
			else
			{
				strcat_t(queryStr, "\"", addComma ? "\"," : "\"");
			}
		}
		else
		{
			strcat_t(queryStr, "\"", addComma ? "\"," : "\"");
		}
	}
	void sqlAddInt(char* queryStr, int size, int value, bool addComma = true);
	void sqlAddInt(string& queryStr, const int value, const bool addComma = true)
	{
		INT_STR(temp, value);
		queryStr += temp.str();
		if (addComma)
		{
			queryStr += ",";
		}
	}
	template<int Length>
	void sqlAddInt(MyString<Length>& queryStr, const int value, const bool addComma = true)
	{
		INT_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, temp.str());
		}
	}
	void sqlAddUInt(char* queryStr, int size, int value, bool addComma = true);
	template<int Length>
	void sqlAddUInt(MyString<Length>& queryStr, const int value, const bool addComma = true)
	{
		UINT_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, temp.str());
		}
	}
	void sqlAddFloat(char* queryStr, int size, float value, bool addComma = true);
	void sqlAddFloat(string& queryStr, const float value, const bool addComma = true)
	{
		FLOAT_STR(temp, value);
		queryStr += temp.str();
		if (addComma)
		{
			queryStr += ",";
		}
	}
	template<int Length>
	void sqlAddFloat(MyString<Length>& queryStr, const float value, const bool addComma = true)
	{
		FLOAT_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, temp.str());
		}
	}
	void sqlAddULLong(char* queryStr, int size, ullong value, bool addComma = true);
	template<int Length>
	void sqlAddULLong(MyString<Length>& queryStr, const ullong value, const bool addComma = true)
	{
		ULLONG_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, temp.str());
		}
	}
	void sqlAddLLong(char* queryStr, int size, llong value, bool addComma = true);
	template<int Length>
	void sqlAddLLong(MyString<Length>& queryStr, const llong value, const bool addComma = true)
	{
		LLONG_STR(temp, value);
		if (addComma)
		{
			strcat_t(queryStr, temp.str(), ",");
		}
		else
		{
			strcat_s(queryStr, temp.str());
		}
	}
	void sqlAddLLong(string& queryStr, const llong value, const bool addComma = true)
	{
		LLONG_STR(temp, value);
		queryStr += temp.str();
		if (addComma)
		{
			queryStr += ",";
		}
	}
	template<int Length>
	void sqlAddBytes(MyString<Length>& queryStr, const Vector<byte>& byteArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(byteArray.size());
		CharArrayScopeThread charArray(arrayLen);
		bytesToString(charArray.mArray, arrayLen, byteArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddBytes(string& queryStr, const Vector<byte>& byteArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(byteArray.size());
		CharArrayScopeThread charArray(arrayLen);
		bytesToString(charArray.mArray, arrayLen, byteArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlAddUShorts(MyString<Length>& queryStr, const Vector<ushort>& ushortArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(ushortArray.size());
		CharArrayScopeThread charArray(arrayLen);
		USsToS(charArray.mArray, arrayLen, ushortArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddUShorts(string& queryStr, const Vector<ushort>& ushortArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(ushortArray.size());
		CharArrayScopeThread charArray(arrayLen);
		USsToS(charArray.mArray, arrayLen, ushortArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlAddInts(MyString<Length>& queryStr, const Vector<int>& intArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		IsToS(charArray.mArray, arrayLen, intArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddInts(string& queryStr, const Vector<int>& intArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		IsToS(charArray.mArray, arrayLen, intArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlAddUInts(MyString<Length>& queryStr, const Vector<uint>& intArray, bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		UIsToS(charArray.mArray, arrayLen, intArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddUInts(string& queryStr, const Vector<uint>& intArray, bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		UIsToS(charArray.mArray, arrayLen, intArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlAddFloats(MyString<Length>& queryStr, const Vector<float>& floatArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(floatArray.size());
		CharArrayScopeThread charArray(arrayLen);
		FsToS(charArray.mArray, arrayLen, floatArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddFloats(string& queryStr, const Vector<float>& floatArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(floatArray.size());
		CharArrayScopeThread charArray(arrayLen);
		FsToS(charArray.mArray, arrayLen, floatArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlAddULLongs(MyString<Length>& queryStr, const Vector<ullong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		ULLsToS(charArray.mArray, arrayLen, longArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddULLongs(string& queryStr, const Vector<ullong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		ULLsToS(charArray.mArray, arrayLen, longArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlAddLLongs(MyString<Length>& queryStr, const Vector<llong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		LLsToS(charArray.mArray, arrayLen, longArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddLLongs(string& queryStr, const Vector<llong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		LLsToS(charArray.mArray, arrayLen, longArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	void sqlAddVector2Int(char* queryStr, int size, const Vector2Int& value, bool addComma = true);
	template<int Length>
	void sqlAddVector2Int(MyString<Length>& queryStr, const Vector2Int& value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	void sqlAddVector2Int(string& queryStr, const Vector2Int& value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	void sqlAddVector2UInt(char* queryStr, int size, const Vector2UInt& value, bool addComma = true);
	template<int Length>
	void sqlAddVector2UInt(MyString<Length>& queryStr, const Vector2UInt& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2UIToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	void sqlAddVector2UInt(string& queryStr, const Vector2UInt& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2UIToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	void sqlAddVector2Short(char* queryStr, int size, const Vector2Short& value, bool addComma = true);
	template<int Length>
	void sqlAddVector2Short(MyString<Length>& queryStr, const Vector2Short& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2SToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	void sqlAddVector2Short(string& queryStr, const Vector2Short& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2SToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	void sqlAddVector2UShort(char* queryStr, int size, const Vector2UShort& value, bool addComma = true);
	template<int Length>
	void sqlAddVector2UShort(MyString<Length>& queryStr, const Vector2UShort& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	void sqlAddVector2UShort(string& queryStr, const Vector2UShort& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	// where条件相关
	void sqlConditionString(string& condition, const string& col, const char* str)
	{
		condition.reserve(condition.size() + col.length() + strlength(str) + 4);
		condition += col;
		condition += "=\"";
		if (checkSQLString(str))
		{
			condition += str;
		}
		condition += "\"";
	}
	void sqlConditionString(string& condition, const string& col, const string& str)
	{
		condition.reserve(condition.size() + col.length() + str.length() + 4);
		condition += col;
		condition += "=\"";
		if (checkSQLString(str))
		{
			condition += str;
		}
		condition += "\"";
	}
	template<int Length>
	void sqlConditionString(MyString<Length>& condition, const string& col, const char* str)
	{
		if (checkSQLString(str))
		{
			strcat_t(condition, col.c_str(), "=\"", str, "\"");
		}
		else
		{
			strcat_t(condition, col.c_str(), "=\"", "\"");
		}
	}
	template<int Length>
	void sqlConditionString(MyString<Length>& condition, const string& col, const char* str, const char* relationalOperator, const char* logicalOperator)
	{
		if (checkSQLString(str))
		{
			strcat_t(condition, col.c_str(), relationalOperator, "\"", str, "\"", logicalOperator);
		}
		else
		{
			strcat_t(condition, col.c_str(), relationalOperator, "\"\"", logicalOperator);
		}
	}
	void sqlConditionString(string& condition, const string& col, const char* str, const char* relationalOperator, const char* logicalOperator)
	{
		condition.reserve(condition.size() + col.length() + strlength(str) + 16);
		condition += col;
		condition += relationalOperator;
		condition += "\"";
		if (checkSQLString(str))
		{
			condition += str;
		}
		condition += "\"";
		condition += logicalOperator;
	}
	void sqlConditionString(string& condition, const string& col, const string& str, const char* relationalOperator, const char* logicalOperator)
	{
		condition.reserve(condition.size() + col.length() + str.size() + 16);
		condition += col;
		condition += relationalOperator;
		condition += "\"";
		if (checkSQLString(str))
		{
			condition += str;
		}
		condition += "\"";
		condition += logicalOperator;
	}
	template<int Length>
	void sqlConditionStringLike(MyString<Length>& condition, const string& col, const char* str)
	{
		if (checkSQLString(str))
		{
			strcat_t(condition, col.c_str(), " LIKE \"%", str, "%\"");
		}
		else
		{
			// 此处不好处理,如果条件带引号,直接去除的话,会导致查询全表
			strcat_t(condition, col.c_str(), " = \"\"");
		}
	}
	template<int Length>
	void sqlConditionStringLike(MyString<Length>& condition, const string& col, const char* str, const char* logicalOperator)
	{
		if (checkSQLString(str))
		{
			strcat_t(condition, col.c_str(), " LIKE \"%", str, "%\"", logicalOperator);
		}
		else
		{
			strcat_t(condition, col.c_str(), " = \"\"", logicalOperator);
		}
	}
	template<int Length>
	void sqlConditionStringLike(MyString<Length>& condition, const string& col, const char* str, const char* logicalOperator, const char* prev, const char* end)
	{
		if (checkSQLString(str))
		{
			strcat_t(condition, col.c_str(), " LIKE \"", prev, str, end, "\"", logicalOperator);
		}
		else
		{
			strcat_t(condition, col.c_str(), " = \"\"", logicalOperator);
		}
	}
	template<int Length>
	void sqlConditionInt(MyString<Length>& condition, const string& col, const int value)
	{
		INT_STR(temp, value);
		strcat_t(condition, col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionInt(MyString<Length>& condition, const string& col, const int value, const char* relationalOperator)
	{
		INT_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionInt(MyString<Length>& condition, const string& col, const int value, const char* relationalOperator, const char* logicalOperator)
	{
		INT_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	template<int Length>
	void sqlConditionUInt(MyString<Length>& condition, const string& col, const int value)
	{
		UINT_STR(temp, value);
		strcat_t(condition, col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionUInt(MyString<Length>& condition, const string& col, const int value, const char* relationalOperator)
	{
		UINT_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionUInt(MyString<Length>& condition, const string& col, const int value, const char* relationalOperator, const char* logicalOperator)
	{
		UINT_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	template<int Length>
	void sqlConditionFloat(MyString<Length>& condition, const string& col, const float value)
	{
		FLOAT_STR(temp, value);
		strcat_t(condition, col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionFloat(MyString<Length>& condition, const string& col, const float value, const char* relationalOperator)
	{
		FLOAT_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionFloat(MyString<Length>& condition, const string& col, const float value, const char* relationalOperator, const char* logicalOperator)
	{
		FLOAT_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	template<int Length>
	void sqlConditionULLong(MyString<Length>& condition, const string& col, const ullong value)
	{
		ULLONG_STR(temp, value);
		strcat_t(condition, col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionULLong(MyString<Length>& condition, const string& col, const ullong value, const char* relationalOperator)
	{
		ULLONG_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionULLong(MyString<Length>& condition, const string& col, const ullong value, const char* relationalOperator, const char* logicalOperator)
	{
		ULLONG_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	template<int Length>
	void sqlConditionLLong(MyString<Length>& condition, const string& col, const llong value)
	{
		LLONG_STR(temp, value);
		strcat_t(condition, col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionLLong(MyString<Length>& condition, const string& col, const llong value, const char* relationalOperator)
	{
		LLONG_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionLLong(MyString<Length>& condition, const string& col, const llong value, const char* relationalOperator, const char* logicalOperator)
	{
		LLONG_STR(temp, value);
		strcat_t(condition, col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	void sqlConditionLLong(string& condition, const string& col, const llong value)
	{
		condition.reserve(condition.size() + col.length() + 10);
		LLONG_STR(temp, value);
		condition += col;
		condition += "=";
		condition += temp.str();
	}
	void sqlConditionLLong(string& condition, const string& col, const llong value, const char* relationalOperator)
	{
		condition.reserve(condition.size() + col.length() + 16);
		LLONG_STR(temp, value);
		condition += col;
		condition += relationalOperator;
		condition += temp.str();
	}
	void sqlConditionLLong(string& condition, const string& col, const llong value, const char* relationalOperator, const char* logicalOperator)
	{
		condition.reserve(condition.size() + col.length() + 16);
		LLONG_STR(temp, value);
		condition += col;
		condition += relationalOperator;
		condition += temp.str();
		condition += logicalOperator;
	}
	// update相关
	MICRO_LEGEND_FRAME_API void sqlUpdateString(char* updateInfo, int size, const string& col, const char* str, bool addComma = true);
	MICRO_LEGEND_FRAME_API void sqlUpdateStringUTF8(char* updateInfo, int size, const string& col, const char* str, bool addComma = true);
	MICRO_LEGEND_FRAME_API void sqlUpdateStringUTF8(string& updateInfo, const string& col, const string& str, bool addComma = true);
	template<int Length>
	void sqlUpdateStringUTF8(MyString<Length>& updateInfo, const string& col, const char* str, const bool addComma = true)
	{
		if (checkSQLString(str))
		{
			char* utf8String = nullptr;
			CharArrayScopeThread scope(utf8String, Length);
			ANSIToUTF8(str, utf8String, Length, false);
			strcat_t(updateInfo, col.c_str(), " = \"", utf8String, addComma ? "\"," : "\"");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = \"", addComma ? "\"," : "\"");
		}
	}
	void sqlUpdateString(string& updateInfo, const string& col, const char* str, const bool addComma = true)
	{
		if (checkSQLString(str))
		{
			updateInfo.reserve(updateInfo.size() + col.length() + strlength(str) + 8);
			updateInfo += col;
			updateInfo += " = \"";
			updateInfo += str;
			updateInfo += addComma ? "\"," : "\"";
		}
		else
		{
			updateInfo.reserve(updateInfo.size() + col.length() + 8);
			updateInfo += col;
			updateInfo += " = \"";
			updateInfo += addComma ? "\"," : "\"";
		}
	}
	template<int Length>
	void sqlUpdateString(MyString<Length>& updateInfo, const string& col, const char* str, const bool addComma = true)
	{
		if (checkSQLString(str))
		{
			strcat_t(updateInfo, col.c_str(), " = \"", str, addComma ? "\"," : "\"");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = \"", addComma ? "\"," : "\"");
		}
	}
	template<int Length>
	void sqlUpdateString(MyString<Length>& updateInfo, const string& col, const char* str, const int strLength, const bool addComma = true)
	{
		if (checkSQLString(str))
		{
			strcat_t(updateInfo, col.c_str(), " = \"");
			strcat_s(updateInfo, str, strLength);
			strcat_t(updateInfo, addComma ? "\"," : "\"");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = \"", addComma ? "\"," : "\"");
		}
	}
	void sqlUpdateString(string& updateInfo, const string& col, const string& str, const bool addComma = true)
	{
		if (checkSQLString(str))
		{
			updateInfo.reserve(updateInfo.size() + col.length() + str.size() + 8);
			updateInfo += col;
			updateInfo += " = \"";
			updateInfo += str;
			updateInfo += addComma ? "\"," : "\"";
		}
		else
		{
			updateInfo.reserve(updateInfo.size() + col.length() + 8);
			updateInfo += col;
			updateInfo += " = \"";
			updateInfo += addComma ? "\"," : "\"";
		}
	}
	template<int Length>
	void sqlUpdateBool(MyString<Length>& updateInfo, const string& col, const bool value, const bool addComma = true)
	{
		// bool当成int来存储
		const int intValue = value ? 1 : 0;
		INT_STR(temp, intValue);
		if (addComma)
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str());
		}
	}
	void sqlUpdateBool(string& updateInfo, const string& col, const bool value, const bool addComma = true)
	{
		updateInfo.reserve(updateInfo.size() + col.length() + 8);
		// bool当成int来存储
		const int intValue = value ? 1 : 0;
		INT_STR(temp, intValue);
		updateInfo += col;
		updateInfo += " = ";
		updateInfo += temp.str();
		if (addComma)
		{
			updateInfo += ",";
		}
	}
	template<int Length>
	void sqlUpdateInt(MyString<Length>& updateInfo, const string& col, const int value, const bool addComma = true)
	{
		INT_STR(temp, value);
		if (addComma)
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str());
		}
	}
	void sqlUpdateInt(string& updateInfo, const string& col, const int value, const bool addComma = true)
	{
		updateInfo.reserve(updateInfo.size() + col.length() + 8);
		INT_STR(temp, value);
		updateInfo += col;
		updateInfo += " = ";
		updateInfo += temp.str();
		if (addComma)
		{
			updateInfo += ",";
		}
	}
	template<int Length>
	void sqlUpdateUInt(MyString<Length>& updateInfo, const string& col, const uint value, const bool addComma = true)
	{
		UINT_STR(temp, value);
		if (addComma)
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str());
		}
	}
	void sqlUpdateUInt(string& updateInfo, const string& col, const uint value, const bool addComma = true)
	{
		updateInfo.reserve(updateInfo.size() + col.length() + 8);
		UINT_STR(temp, value);
		updateInfo += col;
		updateInfo += " = ";
		updateInfo += temp.str();
		if (addComma)
		{
			updateInfo += ",";
		}
	}
	template<int Length>
	void sqlUpdateFloat(MyString<Length>& updateInfo, const string& col, const float value, const bool addComma = true)
	{
		FLOAT_STR(temp, value);
		if (addComma)
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str());
		}
	}
	void sqlUpdateFloat(string& updateInfo, const string& col, const float value, const bool addComma = true)
	{
		updateInfo.reserve(updateInfo.size() + col.length() + 8);
		FLOAT_STR(temp, value);
		updateInfo += col;
		updateInfo += " = ";
		updateInfo += temp.str();
		if (addComma)
		{
			updateInfo += ",";
		}
	}
	template<int Length>
	void sqlUpdateULLong(MyString<Length>& updateInfo, const string& col, const ullong value, const bool addComma = true)
	{
		ULLONG_STR(temp, value);
		if (addComma)
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str());
		}
	}
	template<int Length>
	void sqlUpdateLLong(MyString<Length>& updateInfo, const string& col, const llong value, const bool addComma = true)
	{
		LLONG_STR(temp, value);
		if (addComma)
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			strcat_t(updateInfo, col.c_str(), " = ", temp.str());
		}
	}
	void sqlUpdateLLong(string& updateInfo, const string& col, const llong value, const bool addComma = true)
	{
		updateInfo.reserve(updateInfo.size() + col.length() + 16);
		LLONG_STR(temp, value);
		updateInfo += col;
		updateInfo += " = ";
		updateInfo += temp.str();
		if (addComma)
		{
			updateInfo += ",";
		}
	}
	template<int Length>
	void sqlUpdateBytes(MyString<Length>& updateInfo, const string& col, const Vector<byte>& byteArray, const bool addComma = true)
	{
		const int arrayLen = 4 * greaterPower2(byteArray.size());
		CharArrayScopeThread charArray(arrayLen);
		bytesToString(charArray.mArray, arrayLen, byteArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	void sqlUpdateBytes(string& updateInfo, const string& col, const Vector<byte>& byteArray, const bool addComma = true)
	{
		const int arrayLen = 4 * greaterPower2(byteArray.size());
		CharArrayScopeThread charArray(arrayLen);
		bytesToString(charArray.mArray, arrayLen, byteArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateUShorts(MyString<Length>& updateInfo, const string& col, const Vector<ushort>& ushortArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(ushortArray.size());
		CharArrayScopeThread charArray(arrayLen);
		USsToS(charArray.mArray, arrayLen, ushortArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	void sqlUpdateUShorts(string& updateInfo, const string& col, const Vector<ushort>& ushortArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(ushortArray.size());
		CharArrayScopeThread charArray(arrayLen);
		USsToS(charArray.mArray, arrayLen, ushortArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateInts(MyString<Length>& updateInfo, const string& col, const Vector<int>& intArray, bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		IsToS(charArray.mArray, arrayLen, intArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	void sqlUpdateInts(string& updateInfo, const string& col, const Vector<int>& intArray, bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		IsToS(charArray.mArray, arrayLen, intArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateUInts(MyString<Length>& updateInfo, const string& col, const Vector<uint>& intArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		UIsToS(charArray.mArray, arrayLen, intArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	void sqlUpdateUInts(string& updateInfo, const string& col, const Vector<uint>& intArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		UIsToS(charArray.mArray, arrayLen, intArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateFloats(MyString<Length>& updateInfo, const string& col, const Vector<float>& floatArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(floatArray.size());
		CharArrayScopeThread charArray(arrayLen);
		FsToS(charArray.mArray, arrayLen, floatArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	void sqlUpdateFloats(string& updateInfo, const string& col, const Vector<float>& floatArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(floatArray.size());
		CharArrayScopeThread charArray(arrayLen);
		FsToS(charArray.mArray, arrayLen, floatArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateULLongs(MyString<Length>& updateInfo, const string& col, const Vector<ullong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		ULLsToS(charArray.mArray, arrayLen, longArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	void sqlUpdateULLongs(string& updateInfo, const string& col, const Vector<ullong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		ULLsToS(charArray.mArray, arrayLen, longArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateLLongs(MyString<Length>& updateInfo, const string& col, const Vector<llong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		LLsToS(charArray.mArray, arrayLen, longArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	void sqlUpdateLLongs(string& updateInfo, const string& col, const Vector<llong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		LLsToS(charArray.mArray, arrayLen, longArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateVector2Int(MyString<Length>& updateInfo, const string& col, const Vector2Int& value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	void sqlUpdateVector2Int(string& updateInfo, const string& col, const Vector2Int& value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	template<int Length>
	void sqlUpdateVector2UInt(MyString<Length>& updateInfo, const string& col, const Vector2UInt& value, const bool addComma = true)
	{
		MyString<32> temp;
		V2UIToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	void sqlUpdateVector2UInt(string& updateInfo, const string& col, const Vector2UInt& value, const bool addComma = true)
	{
		MyString<32> temp;
		V2UIToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	template<int Length>
	void sqlUpdateVector2UShort(MyString<Length>& updateInfo, const string& col, const Vector2UShort& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	void sqlUpdateVector2UShort(string& updateInfo, const string& col, const Vector2UShort& value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	MICRO_LEGEND_FRAME_API int base64_encode(const byte* str, int length, char* encode);
	// base64编码,返回值是encode的长度
	MICRO_LEGEND_FRAME_API int base64_encode(const string& str, char* encode);
	// base64解码,返回值是dest的长度
	MICRO_LEGEND_FRAME_API int base64_decode(const char* code, int codeLen, byte* dest);
	// 计算一个字符串的SHA-1值
	MICRO_LEGEND_FRAME_API uint32_t rotate_left(uint32_t value, int shift);
	MICRO_LEGEND_FRAME_API vector<uint8_t> pad_message(const string& message);
	MICRO_LEGEND_FRAME_API uint32_t bytes_to_uint32(const uint8_t* bytes);
	MICRO_LEGEND_FRAME_API void uint32_to_bytes(uint32_t value, uint8_t* bytes);
	MICRO_LEGEND_FRAME_API void sha1(const string& message, byte* buffer);
	MICRO_LEGEND_FRAME_API string sha1(const string& str);
}

using StringUtility::sqlConditionInt;
using StringUtility::sqlConditionFloat;
using StringUtility::sqlConditionString;
using StringUtility::sqlConditionUInt;
using StringUtility::sqlConditionLLong;
using StringUtility::sqlConditionULLong;
using StringUtility::sqlConditionStringLike;
using StringUtility::sqlUpdateInt;
using StringUtility::sqlUpdateString;
using StringUtility::sqlUpdateLLong;
using StringUtility::sqlUpdateBool;
using StringUtility::sqlUpdateFloat;
using StringUtility::sqlAddInt;
using StringUtility::sqlAddFloat;
using StringUtility::sqlAddLLong;
using StringUtility::sqlAddString;
using StringUtility::sqlAddStringUTF8;
using StringUtility::ANSIToUTF8;
using StringUtility::ANSIToUTF8Auto;
using StringUtility::UTF8ToANSI;
using StringUtility::UTF8ToANSIAuto;
using StringUtility::ANSIToUnicode;
using StringUtility::IToS;
using StringUtility::LLToS;
using StringUtility::V3ToS;
using StringUtility::V3IToS;
using StringUtility::V2ToS;
using StringUtility::LLsToS;
using StringUtility::IsToS;
using StringUtility::strcat_t;
using StringUtility::SToLLs;
using StringUtility::SToIs;
using StringUtility::SToLL;
using StringUtility::split;
using StringUtility::splitLine;
using StringUtility::splitFull;
using StringUtility::SToI;
using StringUtility::FToS;
using StringUtility::SToF;
using StringUtility::getFileName;
using StringUtility::removeAll;
using StringUtility::SToV2I;
using StringUtility::SToV2UI;
using StringUtility::SToV2US;
using StringUtility::strlength;
using StringUtility::UIToS;
using StringUtility::endWith;
using StringUtility::hexCharToByte;
using StringUtility::SToV2S;
using StringUtility::isNumber;
using StringUtility::hasInvisibleChar;
using StringUtility::replaceAll;
using StringUtility::startWith;
using StringUtility::findString;
using StringUtility::SToUSs;
using StringUtility::getFileNameNoSuffix;
using StringUtility::getLastNotChar;
using StringUtility::ULLToS;
using StringUtility::USsToS;
using StringUtility::UIsToS;
using StringUtility::FsToS;
using StringUtility::ULLsToS;
using StringUtility::replace;
using StringUtility::strchar;
using StringUtility::sqlUpdateStringUTF8;
using StringUtility::bytesToHexString;
using StringUtility::getFilePath;
using StringUtility::SToBs;
using StringUtility::SToUIs;
using StringUtility::SToFs;
using StringUtility::SToV2;
using StringUtility::SToV3;
using StringUtility::SToV3I;
using StringUtility::SToSs;
using StringUtility::SsToS;
using StringUtility::removeStartString;
using StringUtility::rightToLeft;
#ifdef LINUX
using StringUtility::mSetLocaleLock;
#endif
using StringUtility::toUpper;
using StringUtility::checkString;
using StringUtility::getCharCount;
using StringUtility::POWER_INT_10;
using StringUtility::INVERSE_POWER_INT_10;
using StringUtility::POWER_LLONG_10;
using StringUtility::INVERSE_POWER_LLONG_10;
using StringUtility::V2IToS;
using StringUtility::V2UIToS;
using StringUtility::byteToHexChar;
using StringUtility::removeLLongsString;
using StringUtility::stringsToString;
using StringUtility::BToS;
using StringUtility::SToB;
using StringUtility::SToV2s;
using StringUtility::SToV2Is;
using StringUtility::SToV3s;
using StringUtility::SToV3Is;
using StringUtility::base64_encode;
using StringUtility::sha1;