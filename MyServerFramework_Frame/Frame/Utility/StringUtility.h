#pragma once

#include "BinaryUtility.h"
#include "ThreadLock.h"
#include "CharArrayScopeThread.h"
#include "MyString.h"
#include "MyCharArray.h"

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
	inline void removeLastComma(string& stream) { removeLast(stream, ','); }
	MICRO_LEGEND_FRAME_API int strlength(const char* str, int maxLength);
	MICRO_LEGEND_FRAME_API int strlength(const char* str);
	// 查找str中指定key的数量
	MICRO_LEGEND_FRAME_API int findCharCount(const string& str, char key);
	MICRO_LEGEND_FRAME_API int strchar(const char* str, char key, int startIndex = 0, int length = 0);
	MICRO_LEGEND_FRAME_API string getFileName(const string& str);
	MICRO_LEGEND_FRAME_API string getFileNameNoSuffix(const string& str, bool removePath);
	MICRO_LEGEND_FRAME_API string getFirstFolderName(const string& dir);
	MICRO_LEGEND_FRAME_API string removeFirstPath(const string& dir);
	MICRO_LEGEND_FRAME_API string getFilePath(const string& dir, bool keepSlash);
	MICRO_LEGEND_FRAME_API string getFileSuffix(const string& fileName, bool keepDot);
	MICRO_LEGEND_FRAME_API string removeStartString(const string& fileName, const string& startStr);
	MICRO_LEGEND_FRAME_API string removeEndString(const string& fileName, const string& endStr);
	inline string replaceSuffix(const string& fileName, const string& suffix) { return getFileNameNoSuffix(fileName, false) + suffix; }
	// 获得字符串最后不是数字的下标
	MICRO_LEGEND_FRAME_API int getLastNotNumberPos(const string& str);
	// 获得字符串结尾的数字
	MICRO_LEGEND_FRAME_API int getLastNumber(const string& str);
	// 获得去除末尾数字以后的字符串
	inline string getNotNumberSubString(const string& str) { return str.substr(0, getLastNotNumberPos(str) + 1); }
	MICRO_LEGEND_FRAME_API int getLastChar(const char* str, char value);
	MICRO_LEGEND_FRAME_API int getLastNotChar(const string& str, char value);
	MICRO_LEGEND_FRAME_API void splitLine(const char* str, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void splitLine(const char* str, string* stringBuffer, int bufferSize, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void split(const char* str, char key, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API Vector<string> split(const char* str, char key, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API int split(const char* str, char key, string* stringBuffer, int bufferSize, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void split(const char* str, const char* key, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void split(const string& str, const char* key, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API void split(const string& str, const char key, Vector<string>& vec, bool removeEmpty = true);
	MICRO_LEGEND_FRAME_API int split(const char* str, const char* key, string* stringBuffer, int bufferSize, bool removeEmpty = true);
	// 将字符串全部转为小写再查找
	MICRO_LEGEND_FRAME_API bool findStringLower(const string& res, const string& sub, int* pos = nullptr, int startIndex = 0);
	MICRO_LEGEND_FRAME_API bool findString(const string& res, const char* sub, int* pos = nullptr, int startIndex = 0);
	// 从后往前查找
	MICRO_LEGEND_FRAME_API bool findStringInverse(const string& res, const char* sub, int* pos = nullptr);
	MICRO_LEGEND_FRAME_API bool findString(const char* res, const char* sub, int* pos = nullptr, int startIndex = 0);
	MICRO_LEGEND_FRAME_API bool findString(const char* res, char key, int* pos = nullptr, int startIndex = 0);
	MICRO_LEGEND_FRAME_API int findStringPos(const char* res, const char* sub, int startIndex = 0);
	MICRO_LEGEND_FRAME_API int findStringPos(const string& res, const string& sub, int startIndex = 0);
	// 将str中的[begin,end)替换为reStr
	MICRO_LEGEND_FRAME_API void replace(char* str, int strBufferSize, int begin, int end, const char* reStr);
	template<int Length>
	void replace(MyString<Length>& str, const int begin, const int end, const char* reStr)
	{
		replace(str.data(), Length, begin, end, reStr);
	}
	MICRO_LEGEND_FRAME_API void replace(string& str, int begin, int end, const string& reStr);
	template<int Length>
	void replaceAll(MyString<Length>& str, const char* key, const char* newWord)
	{
		replaceAll(str.data(), Length, key, newWord);
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
		memmove(str.data() + subPos, str.data() + subPos + subLength, Length - subLength - subPos);
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
	template<int SourceLength>
	void strcat_s(char* destBuffer, const int destSize, const Array<SourceLength, const char*>& sourceArray)
	{
		int destIndex = strlength(destBuffer, destSize);
		for (const char* curSource : sourceArray)
		{
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
	MICRO_LEGEND_FRAME_API void strcpy_s(char* destBuffer, int size, const char* source);
	// 以string类型返回count个0
	MICRO_LEGEND_FRAME_API string zeroString(int zeroCount);
	// 基础数据类型转换为字符串
	//-----------------------------------------------------------------------------------------------------------------------------
	inline string BToS(const bool value) { return value ? "True" : "False"; }
	// 返回string类型的数字字符串,速度较慢
	MICRO_LEGEND_FRAME_API string LLToS(llong value, int limitLen = 0);
	template<int Length>
	int _i64toa_s_(llong value, MyString<Length>& charArray)
	{
		charArray.clear();
		if (value == 0)
		{
			charArray.set('0');
			return charArray.length();
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
		MyString<Length> temp;
		while (true)
		{
			const int index = temp.length();
			// 将数字放入temp
			if (value < POWER_LLONG_10[index] ||
				!temp.add((int)(value % POWER_LLONG_10[index + 1] / POWER_LLONG_10[index]) + '0'))
			{
				break;
			}
		}
		// 将数字从数组末尾移动到开头
		if (sign < 0)
		{
			charArray.add('-');
		}
		FOR(temp.length())
		{
			charArray.add(temp[temp.length() - i - 1]);
		}
		return charArray.length();
	}
	template<int Length>
	int LLToS(MyString<Length>& charArray, const llong value, const int limitLen = 0)
	{
		charArray.clear();
		if (limitLen == 0)
		{
			return _i64toa_s_(value, charArray);
		}
		MyString<32> temp;
		const int len = _i64toa_s_(value, temp);
		// 判断是否需要在前面补0
		if (limitLen > len)
		{
			charArray.setCount('0', limitLen - len);
			charArray.add(temp.str());
		}
		else
		{
			charArray.set(temp.str(), len);
		}
		return charArray.length();
	}
	template<int Length>
	int _itoa_s(int value, MyString<Length>& charArray)
	{
		charArray.clear();
		if (value == 0)
		{
			charArray.set('0');
			return charArray.length();
		}

		if (mIntString[0].length() == 0)
		{
			initIntToString();
		}
		// 优先查表,但是前提是表已经有值
		if (!mIntString[mIntString.size() - 1].empty() && value >= 0 && value < mIntString.size())
		{
			const string& str = mIntString[value];
			const int strLength = (int)str.length();
			if (strLength + 1 >= (int)Length)
			{
				ERROR("int value is too large:" + LLToS((llong)value));
				return 0;
			}
			charArray.set(str);
			return charArray.length();
		}

		int sign = 1;
		unsigned int absValue = (unsigned int)value;
		if (value < 0)
		{
			// 用 unsigned int 保存绝对值，避免 INT_MIN 取负溢出
			absValue = (uint)(-(llong)value);
			sign = -1;
		}
		if ((llong)absValue > POWER_INT_10[POWER_INT_10.size() - 1])
		{
			ERROR("int value is too large:" + LLToS((llong)value));
			return 0;
		}
		MyString<Length> temp;
		while (true)
		{
			const int index = temp.length();
			// 将数字放入temp
			if ((llong)absValue < POWER_INT_10[index] ||
				!temp.add((int)((llong)absValue % POWER_INT_10[index + 1] / POWER_INT_10[index]) + '0'))
			{
				break;
			}
		}
		// 将数字从数组末尾移动到开头
		if (sign < 0)
		{
			charArray.add('-');
		}
		FOR(temp.length())
		{
			charArray.add(temp[temp.length() - i - 1]);
		}
		return charArray.length();
	}
	// 这里有问题,应该是
	template<int Length>
	int _uitoa_s(const uint value, MyString<Length>& charArray)
	{
		charArray.clear();
		if (value == 0)
		{
			charArray.set('0');
			return charArray.length();
		}

		if (mIntString[0].length() == 0)
		{
			initIntToString();
		}
		// 优先查表,但是前提是表已经有值
		if (!mIntString[mIntString.size() - 1].empty() && value < (uint)mIntString.size())
		{
			const string& str = mIntString[value];
			const int strLength = (int)str.length();
			if (strLength + 1 >= (int)Length)
			{
				ERROR("uint value is too large:" + LLToS((llong)value));
				return 0;
			}
			charArray.set(str, strLength);
			return charArray.length();
		}

		if ((llong)value > POWER_INT_10[POWER_INT_10.size() - 1])
		{
			ERROR("uint value is too large:" + LLToS((llong)value));
			return 0;
		}
		MyString<Length> temp;
		while (true)
		{
			const int index = temp.length();
			// 将数字放入temp
			if ((llong)value < POWER_INT_10[index] ||
				!temp.add((int)((llong)value % POWER_INT_10[index + 1] / POWER_INT_10[index]) + '0'))
			{
				break;
			}
		}
		// 将数字从数组末尾移动到开头
		FOR(temp.length())
		{
			charArray.add(temp[temp.length() - i - 1]);
		}
		return charArray.length();
	}
	template<int Length>
	int _ui64toa_s(const ullong value, MyString<Length>& charArray)
	{
		charArray.clear();
		if (value == 0)
		{
			charArray.set('0');
			return charArray.length();
		}
		if (value > (ullong)POWER_LLONG_10[POWER_LLONG_10.size() - 1])
		{
			ERROR("ullong value is too large");
			return 0;
		}
		MyString<Length> temp;
		while (true)
		{
			const int index = temp.length();
			// 将数字放入temp
			if (value < (ullong)POWER_LLONG_10[index] ||
				!temp.add((int)(value % POWER_LLONG_10[index + 1] / POWER_LLONG_10[index]) + '0'))
			{
				break;
			}
		}
		// 将数字从数组末尾移动到开头
		FOR(temp.length())
		{
			charArray.add(temp[temp.length() - i - 1]);
		}
		return charArray.length();
	}
	// 返回string类型的数字字符串,速度较慢,limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	MICRO_LEGEND_FRAME_API string UIToS(int value, int limitLen = 0);
	template<int Length>
	int UIToS(MyString<Length>& charArray, const int value, const int limitLen = 0)
	{
		charArray.clear();
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
			charArray.setCount('0', limitLen - len);
			charArray.add(temp.str());
		}
		else
		{
			charArray.set(temp.str(), len);
		}
		return charArray.length();
	}
	// 返回string类型的数字字符串,速度较慢,limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	MICRO_LEGEND_FRAME_API string IToS(const int value, int limitLen = 0);
	template<int Length>
	int IToS(MyString<Length>& charArray, const int value, const int limitLen = 0)
	{
		charArray.clear();
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
			charArray.setCount('0', limitLen - len);
			charArray.add(temp.str());
		}
		else
		{
			charArray.set(temp.str(), len);
		}
		return charArray.length();
	}
	// 返回string类型的数字字符串,速度较慢
	MICRO_LEGEND_FRAME_API string ULLToS(ullong value, int limitLen = 0);
	template<int Length>
	int ULLToS(MyString<Length>& charArray, const ullong value, const int limitLen = 0)
	{
		charArray.clear();
		if (limitLen == 0)
		{
			return _ui64toa_s(value, charArray);
		}
		MyString<32> temp;
		const int len = _ui64toa_s(value, temp);
		// 判断是否需要在前面补0
		if (limitLen > len)
		{
			charArray.setCount('0', limitLen - len);
			charArray.add(temp.str());
		}
		else
		{
			charArray.set(temp.str(), len);
		}
		return charArray.length();
	}
	// precision为精度,保留的小数的位数,removeZero为是否去除末尾无用的0,速度较慢
	MICRO_LEGEND_FRAME_API string floatToStringExtra(float f, int precision = 4, bool removeTailZero = true);
	// 将浮点数转换为字符串,速度较快
	MICRO_LEGEND_FRAME_API string FToS(float f, int precision = 4);
	template<int Length>
	int FToS(MyString<Length>& charArray, float f, int precision = 4)
	{
		charArray.clear();
		if (f > 99999999.0f)
		{
			f = 99999999.0f;
		}
		else if (f < -99999999.0f)
		{
			f = -99999999.0f;
		}
		const char* PRECISION[6] = { "%.0f", "%.1f", "%.2f", "%.3f", "%.4f", "%.5f" };
		if (precision > 5)
		{
			precision = 5;
		}
		else if (precision < 0)
		{
			precision = 0;
		}
		charArray.sprintf(PRECISION[precision], f);
		// 先查找小数点和结束符的位置
		int dotPos = -1;
		int strLen = 0;
		FOR(Length)
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
			FOR(strLen)
			{
				const int index = strLen - 1 - i;
				// 如果找到了小数点仍然没有找到一个不为'0'的字符,则从小数点部分截断字符串
				if (index == dotPos)
				{
					charArray.truncate(dotPos);
					break;
				}
				// 找到小数点后的从后往前的第一个不为'0'的字符,从此处截断字符串
				if (charArray[index] != '0' && index + 1 <= strLen)
				{
					charArray.truncate(index + 1);
					break;
				}
			}
		}
		return charArray.length();
	}
	MICRO_LEGEND_FRAME_API string bytesToString(const char* buffer, int length);
	inline string V2ToS(Vector2 vec, const char* separate = ",") { return FToS(vec.x) + separate + FToS(vec.y); }
	template<int Length>
	void V2ToS(MyString<Length>& buffer, Vector2 vec, const char* separate = ",")
	{
		buffer.clear();
		FLOAT_STR(xStr, vec.x);
		FLOAT_STR(yStr, vec.y);
		buffer.add(xStr.str(), separate, yStr.str());
	}
	inline string V3ToS(const Vector3& vec, const char* separate = ",") { return FToS(vec.x) + separate + FToS(vec.y) + separate + FToS(vec.z); }
	template<int Length>
	void V3ToS(MyString<Length>& buffer, const Vector3& vec, const char* separate = ",")
	{
		buffer.clear();
		FLOAT_STR(xStr, vec.x);
		FLOAT_STR(yStr, vec.y);
		FLOAT_STR(zStr, vec.z);
		buffer.add(xStr.str(), separate, yStr.str(), separate, zStr.str());
	}
	inline string V3IToS(const Vector3Int& vec, const char* separate = ",") { return IToS(vec.x) + separate + IToS(vec.y) + separate + IToS(vec.z); }
	template<int Length>
	void V3IToS(MyString<Length>& buffer, const Vector3Int& vec, const char* separate = ",")
	{
		buffer.clear();
		INT_STR(xStr, vec.x);
		INT_STR(yStr, vec.y);
		INT_STR(zStr, vec.z);
		buffer.add(xStr.str(), separate, yStr.str(), separate, zStr.str());
	}
	inline string V2IToS(Vector2Int value, const char* separate = ",") { return IToS(value.x) + separate + IToS(value.y); }
	MICRO_LEGEND_FRAME_API string V2IsToS(const Vector<Vector2Int>& value, const char* separate = "|");
	MICRO_LEGEND_FRAME_API string V2IsToS(const HashMap<int, int>& value, const char* separate = "|");
	inline string V2UIToS(Vector2UInt value, const char* separate = ",") { return UIToS(value.x) + separate + UIToS(value.y); }
	template<int Length>
	void V2IToS(MyString<Length>& buffer, Vector2Int value, const char* separate = ",")
	{
		buffer.clear();
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		buffer.add(xStr.str(), separate, yStr.str());
	}
	template<int Length>
	void V2UIToS(MyString<Length>& buffer, Vector2UInt value, const char* separate = ",")
	{
		buffer.clear();
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		buffer.add(xStr.str(), separate, yStr.str());
	}
	inline string V2SToS(Vector2Short value, const char* separate = ",") { return IToS(value.x) + separate + IToS(value.y); }
	template<int Length>
	void V2SToS(MyString<Length>& buffer, Vector2Short value, const char* separate = ",")
	{
		buffer.clear();
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		buffer.add(xStr.str(), separate, yStr.str());
	}
	inline string V2USToS(Vector2UShort value, const char* separate = ",") { return IToS(value.x) + separate + IToS(value.y); }
	template<int Length>
	void V2USToS(MyString<Length>& buffer, Vector2UShort value, const char* separate = ",")
	{
		buffer.clear();
		INT_STR(xStr, value.x);
		INT_STR(yStr, value.y);
		buffer.add(xStr.str(), separate, yStr.str());
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	// 字符串转换为基础数据类型
	//-----------------------------------------------------------------------------------------------------------------------------
	inline bool SToB(const string& str) { return str == "True" || str == "true"; }
	inline bool SToB(const char* str) { return strcmp(str, "True") == 0 || strcmp(str, "true") == 0; }
	inline int SToI(const string& str) { return atoi(str.c_str()); }
	inline int SToI(const char* str) { return atoi(str); }
	inline ullong stringToULLong(const string& str) { return (ullong)atoll(str.c_str()); }
	inline ullong stringToULLong(const char* str) { return (ullong)atoll(str); }
	inline llong SToLL(const string& str) { return atoll(str.c_str()); }
	inline llong SToLL(const char* str) { return atoll(str); }
	inline float SToF(const string& str) { return (float)atof(str.c_str()); }
	inline float SToF(const char* str) { return (float)atof(str); }
	MICRO_LEGEND_FRAME_API Vector2 SToV2(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector2Int SToV2I(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector2UInt SToV2UI(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector2Short SToV2S(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector2UShort SToV2US(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector3 SToV3(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector3Int SToV3I(const string& str, const char* separate = ",");
	//-----------------------------------------------------------------------------------------------------------------------------
	// 基础数据类型数组转换为字符串
	//-----------------------------------------------------------------------------------------------------------------------------
	MICRO_LEGEND_FRAME_API string ULLsToS(const Vector<ullong>& valueList, const char* separate = ",");
	template<int Length>
	string ULLsToS(const Array<Length, ullong>& valueList, const int count = -1, const char* separate = ",")
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
		FOR(count)
		{
			ULLToS(temp, valueList[i]);
			if (i != count - 1)
			{
				strcat_t(charArray.mArray, arrayLen, temp.str(), separate);
			}
			else
			{
				strcat_s(charArray.mArray, arrayLen, temp.str());
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void ULLsToS(MyString<Length>& buffer, const ullong* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}
		MyString<32> temp;
		FOR(count)
		{
			ULLToS(temp, valueList[i]);
			if (i != count - 1)
			{
				buffer.add(temp.str(), separate);
			}
			else
			{
				buffer.add(temp.str());
			}
		}
	}
	template<int Length>
	void ULLsToS(MyString<Length>& buffer, const Vector<ullong>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}
		MyString<32> temp;
		const int count = valueList.size();
		FOR(count)
		{
			ULLToS(temp, valueList[i]);
			if (i != count - 1)
			{
				buffer.add(temp.str(), separate);
			}
			else
			{
				buffer.add(temp.str());
			}
		}
	}
	MICRO_LEGEND_FRAME_API void ULLsToS(char* buffer, const int bufferSize, const Vector<ullong>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API string LLsToS(const Vector<llong>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API string LLsToS(const llong* valueList, int valueCount, const char* separate = ",");
	template<int Length>
	string LLsToS(const Array<Length, llong>& valueList, int count = -1, const char* separate = ",")
	{
		if (count == -1)
		{
			count = (int)Length;
		}
		// 根据列表长度选择适当的数组长度,每个llong默认数字长度为32个字符
		const int arrayLen = 32 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int separateLen = strlength(separate);
		MyString<32> temp;
		FOR(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, separate, separateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	string LLsToS(const ArrayList<Length, llong>& valueList, const char* separate = ",")
	{
		const int count = valueList.size();
		// 根据列表长度选择适当的数组长度,每个llong默认数字长度为32个字符
		const int arrayLen = 32 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int separateLen = strlength(separate);
		MyString<32> temp;
		FOR(count)
		{
			const int len = LLToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, separate, separateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void LLsToS(MyString<Length>& buffer, const llong* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}
		const int separateLen = strlength(separate);
		MyString<32> temp;
		FOR(count)
		{
			const int len = LLToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void LLsToS(MyString<Length>& buffer, const Vector<llong>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}
		const int separateLen = strlength(separate);
		MyString<32> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = LLToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void LLsToS(char* buffer, const int bufferSize, const Vector<llong>& valueList, const char* separate = ",");
	template<int Length, int ValueCount>
	void LLsToS(MyString<Length>& buffer, const Array<ValueCount, llong>& valueList, int count = -1, const char* separate = ",")
	{
		if (count == -1)
		{
			count = (int)ValueCount;
		}
		const int separateLen = strlength(separate);
		buffer.clear();
		MyString<32> temp;
		FOR(count)
		{
			const int len = LLToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	// 将byte数组当成整数数组转换为字符串,并非直接将byte数组转为字符串
	MICRO_LEGEND_FRAME_API string bytesToString(const Vector<byte>& valueList, const char* separate = ",");
	template<int Length>
	void bytesToString(MyString<Length>& buffer, byte* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<8> temp;
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void bytesToString(MyString<Length>& buffer, const Vector<byte>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void bytesToString(char* buffer, const int bufferSize, const Vector<byte>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API string SsToS(const Vector<short>& valueList, const char* separate = ",");
	template<int Length>
	void SsToS(MyString<Length>& buffer, short* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<8> temp;
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void SsToS(MyString<Length>& buffer, const Vector<short>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void SsToS(char* buffer, const int bufferSize, const Vector<short>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API string USsToS(const Vector<ushort>& valueList, const char* separate = ",");
	template<int Length>
	string USsToS(const ArrayList<Length, ushort>& valueList, const char* separate = ",")
	{
		if (valueList.isEmpty())
		{
			return FrameDefine::EMPTY;
		}
		const int arrayLen = 8 * greaterPower2(valueList.size());
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = '\0';
		const int separateLen = strlength(separate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, separate, separateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void USsToS(MyString<Length>& buffer, ushort* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<8> temp;
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void USsToS(MyString<Length>& buffer, const Vector<ushort>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<8> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void USsToS(char* buffer, const int bufferSize, const Vector<ushort>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API string IsToS(const Vector<int>& valueList, const char* separate = ",");
	template<int Length>
	string IsToS(const Array<Length, int>& valueList, const int count, const char* separate = ",")
	{
		if (count == 0)
		{
			return "";
		}
		// 根据列表长度选择适当的数组长度,每个int默认数字长度为16个字符
		const int arrayLen = 16 * greaterPower2(count);
		CharArrayScopeThread charArray(arrayLen);
		charArray.mArray[0] = 0;
		const int separateLen = strlength(separate);
		MyString<16> temp;
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, separate, separateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	string IsToS(const Array<Length, int>& valueList, const char* separate = ",")
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
		const int separateLen = strlength(separate);
		MyString<16> temp;
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, separate, separateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	string IsToS(const ArrayList<Length, int>& valueList, const char* separate = ",")
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
		const int separateLen = strlength(separate);
		MyString<16> temp;
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(charArray.mArray, arrayLen, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(charArray.mArray, arrayLen, separate, separateLen);
			}
		}
		return charArray.mArray;
	}
	template<int Length>
	void IsToS(MyString<Length>& buffer, int* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<16> temp;
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void IsToS(MyString<Length>& buffer, const Vector<int>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void IsToS(char* buffer, int bufferSize, const Vector<int>& valueList, const char* separate = ",");
	template<int Length>
	void IsToS(char* buffer, int bufferSize, const ArrayList<Length, int>& valueList, const char* separate = ",")
	{
		buffer[0] = '\0';
		if (valueList.isEmpty())
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = IToS(temp, valueList[i]);
			strcat_s(buffer, bufferSize, temp.str(), len);
			if (i != count - 1)
			{
				strcat_s(buffer, bufferSize, separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API string UIsToS(const Vector<uint>& valueList, const char* separate = ",");
	template<int Length>
	void UIsToS(MyString<Length>& buffer, uint* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<16> temp;
		FOR(count)
		{
			const int len = UIToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void UIsToS(MyString<Length>& buffer, const Vector<uint>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = UIToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void UIsToS(char* buffer, int bufferSize, const Vector<uint>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API string FsToS(const Vector<float>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API void FsToS(char* buffer, int bufferSize, const Vector<float>& valueList, const char* separate = ",");
	template<int Length>
	void FsToS(MyString<Length>& buffer, float* valueList, const int count, const char* separate = ",")
	{
		buffer.clear();
		if (valueList == nullptr || count == 0)
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<16> temp;
		FOR(count)
		{
			const int len = FToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void FsToS(MyString<Length>& buffer, const Vector<float>& valueList, const char* separate = ",")
	{
		buffer.clear();
		if (valueList.isEmpty())
		{
			return;
		}

		const int separateLen = strlength(separate);
		MyString<16> temp;
		const int count = valueList.size();
		FOR(count)
		{
			const int len = FToS(temp, valueList[i]);
			buffer.add(temp.str(), len);
			if (i != count - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	template<int Length>
	void stringsToString(MyString<Length>& buffer, const char** strList, const int stringCount, const char* separate = ",")
	{
		const int separateLen = strlength(separate);
		buffer.clear();
		FOR(stringCount)
		{
			buffer.add(strList[i]);
			if (i != stringCount - 1)
			{
				buffer.add(separate, separateLen);
			}
		}
	}
	MICRO_LEGEND_FRAME_API string stringsToString(const Vector<string>& strList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API string stringsToString(const Set<string>& strList, const char* separate = ",");
	//-----------------------------------------------------------------------------------------------------------------------------
	// 字符串转换为基础数据类型数组
	//-----------------------------------------------------------------------------------------------------------------------------
	MICRO_LEGEND_FRAME_API void SToBs(const string& str, Vector<byte>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API int SToBs(const char* str, byte* buffer, int bufferSize, const char* separate = ",");
	template<int Length>
	int SToBs(const char* str, Array<Length, byte>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int keyLen = strlength(separate);
		const int sourceLen = strlength(str);
		MyString<4> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void SToSs(const string& str, Vector<short>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API int SToSs(const char* str, short* buffer, int bufferSize, const char* separate = ",");
	template<int Length>
	int SToSs(const string& str, Array<Length, short>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(separate);
		MyString<16> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str.c_str() + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void SToUSs(const string& str, Vector<ushort>& valueList, const char* separate = ",");
	template<int Length>
	void SToUSs(const string& str, ArrayList<Length, ushort>& valueList, const char* separate = ",")
	{
		Vector<string> strList;
		split(str, separate, strList);
		valueList.clear();
		for (const string& curStr : strList)
		{
			if (!curStr.empty())
			{
				valueList.add(SToI(curStr));
			}
		}
	}
	MICRO_LEGEND_FRAME_API int SToUSs(const char* str, ushort* buffer, int bufferSize, const char* separate = ",");
	template<int Length>
	int SToUSs(const char* str, Array<Length, ushort>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(separate);
		MyString<8> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void SToIs(const string& str, Vector<int>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector<int> SToIs(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API int SToIs(const char* str, int* buffer, int bufferSize, const char* separate = ",");
	template<int Length>
	int SToIs(const string& str, Array<Length, int>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(separate);
		MyString<16> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str.c_str() + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为整数放入列表
			if (curString.isEmpty())
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
	void SToIs(const string& str, ArrayList<Length, int>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(separate);
		MyString<16> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str.c_str() + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为整数放入列表
			if (curString.isEmpty())
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
	int SToIs(const char* str, Array<Length, int>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(separate);
		MyString<16> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void SToUIs(const string& str, Vector<uint>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API int SToUIs(const char* str, uint* buffer, int bufferSize, const char* separate = ",");
	template<int Length>
	int SToUIs(const char* str, Array<Length, uint>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(separate);
		MyString<16> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为长整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void stringToULLongs(const char* str, Vector<ullong>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API int stringToULLongs(const char* str, ullong* buffer, int bufferSize, const char* separate = ",");
	template<int Length>
	int stringToULLongs(const char* str, Array<Length, ullong>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(separate);
		MyString<32> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为长整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void SToLLs(const char* str, Vector<llong>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API void SToLLs(const string& str, Vector<llong>& valueList, const char* separate = ",");
	MICRO_LEGEND_FRAME_API Vector<llong> SToLLs(const string& str, const char* separate = ",");
	MICRO_LEGEND_FRAME_API int SToLLs(const char* str, llong* buffer, int bufferSize, const char* separate = ",");
	template<int Length>
	int SToLLs(const string& str, Array<Length, llong>& buffer, int destOffset = 0, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = destOffset;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(separate);
		MyString<32> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str.c_str() + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为长整数放入列表
			if (curString.isEmpty())
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
	void SToLLs(const string& str, ArrayList<Length, llong>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		const int sourceLen = (int)str.length();
		const int keyLen = strlength(separate);
		MyString<32> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str.c_str() + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为长整数放入列表
			if (curString.isEmpty())
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
	int SToLLs(const char* str, Array<Length, llong>& buffer, int destOffset = 0, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = destOffset;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(separate);
		MyString<32> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为长整数放入列表
			if (curString.isEmpty())
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
	void SToLLs(const char* str, ArrayList<Length, llong>& buffer, int destOffset = 0, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(separate);
		MyString<32> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为长整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void SToFs(const string& str, Vector<float>& valueList, const char* separate = ",");
	template<int Length>
	int SToFs(const char* str, Array<Length, float>& buffer, const char* separate = ",", const bool showError = true)
	{
		int startPos = 0;
		int curCount = 0;
		const int sourceLen = strlength(str);
		const int keyLen = strlength(separate);
		MyString<32> curString;
		int dividePos = -1;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, separate, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 转换为长整数放入列表
			if (curString.isEmpty())
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
	MICRO_LEGEND_FRAME_API void SToV2Is(const string& str, Vector<Vector2Int>& valueList, const char* separate = "|");
	MICRO_LEGEND_FRAME_API void SToV2Is(const string& str, HashMap<int, int>& valueList, const char* separate = "|");
	MICRO_LEGEND_FRAME_API void SToV2s(const string& str, Vector<Vector2>& valueList, const char* separate = "|");
	MICRO_LEGEND_FRAME_API void SToV3Is(const string& str, Vector<Vector3Int>& valueList, const char* separate = "|");
	MICRO_LEGEND_FRAME_API void SToV3s(const string& str, Vector<Vector3>& valueList, const char* separate = "|");
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
			needRemoveString.add(valueString.str(), ",");
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
			valueArrayString.add(",", idStr.str());
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
			needRemoveString.add(valueString.str(), ",");
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
			valueArrayString.add(",", idStr.str());
		}
		else
		{
			valueArrayString.copy(idStr);
		}
	}
	template<int Length>
	int split(const string& str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		stringBuffer.clear();
		const int sourceLen = (int)str.length();
		if (sourceLen == 0)
		{
			return 0;
		}
		const int keyLen = strlength(key);
		constexpr int STRING_BUFFER = 1024;
		MyString<STRING_BUFFER> curString;
		int dividePos = -1;
		int startPos = 0;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, key, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			if (dividePos - startPos >= STRING_BUFFER)
			{
				ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + IToS(STRING_BUFFER) + "字节");
				return stringBuffer.size();
			}
			curString.set(str.c_str() + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 放入列表
			if (curString.isEmpty() && removeEmpty)
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
		return stringBuffer.size();
	}
	template<int Length>
	int split(const string& str, const char key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		stringBuffer.clear();
		const int sourceLen = (int)str.length();
		if (sourceLen == 0)
		{
			return 0;
		}
		constexpr int STRING_BUFFER = 1024;
		MyString<STRING_BUFFER> curString;
		int dividePos = -1;
		int startPos = 0;
		bool ret = true;
		while (ret)
		{
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = (int)str.find_first_of(key, startPos);
			ret = dividePos >= 0;
			dividePos = ret ? dividePos : sourceLen;
			if (dividePos - startPos >= STRING_BUFFER)
			{
				ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + IToS(STRING_BUFFER) + "字节");
				return stringBuffer.size();
			}
			curString.set(str.c_str() + startPos, dividePos - startPos);
			startPos = dividePos + 1;
			// 放入列表
			if (curString.isEmpty() && removeEmpty)
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
		return stringBuffer.size();
	}
	template<int Length>
	int split(const char* str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		stringBuffer.clear();
		const int sourceLen = strlength(str);
		if (sourceLen == 0)
		{
			return 0;
		}
		const int keyLen = strlength(key);
		constexpr int STRING_BUFFER = 1024;
		MyString<STRING_BUFFER> curString;
		int dividePos = -1;
		int startPos = 0;
		bool ret = true;
		while (ret)
		{
			ret = findString(str, key, &dividePos, startPos);
			// 无论是否查找到,都将前面一段字符串截取出来
			dividePos = ret ? dividePos : sourceLen;
			if (dividePos - startPos >= STRING_BUFFER)
			{
				ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + IToS(STRING_BUFFER) + "字节");
				return stringBuffer.size();
			}
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + keyLen;
			// 放入列表
			if (curString.isEmpty() && removeEmpty)
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
		return stringBuffer.size();
	}
	template<int Length>
	int split(const char* str, const char key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		stringBuffer.clear();
		const int sourceLen = strlength(str);
		if (sourceLen == 0)
		{
			return 0;
		}
		int startPos = 0;
		constexpr int STRING_BUFFER = 1024;
		MyString<STRING_BUFFER> curString;
		bool ret = true;
		while (ret)
		{
			int dividePos = strchar(str, key, startPos, sourceLen);
			// 无论是否查找到,都将前面一段字符串截取出来
			if (dividePos == -1)
			{
				dividePos = sourceLen;
				ret = false;
			}
			if (dividePos - startPos >= STRING_BUFFER)
			{
				ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + IToS(STRING_BUFFER) + "字节");
				return stringBuffer.size();
			}
			curString.set(str + startPos, dividePos - startPos);
			startPos = dividePos + 1;
			// 放入列表
			if (curString.isEmpty() && removeEmpty)
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
		return stringBuffer.size();
	}

	template<int Length>
	bool splitFull(const char* str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		const int elementCount = split(str, key, stringBuffer, removeEmpty, showError);
		return elementCount == stringBuffer.maxSize();
	}
	template<int Length>
	bool splitFull(const string& str, const char* key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		const int elementCount = split(str, key, stringBuffer, removeEmpty, showError);
		return elementCount == stringBuffer.maxSize();
	}
	template<int Length>
	bool splitFull(const string& str, char key, ArrayList<Length, string>& stringBuffer, const bool removeEmpty = true, const bool showError = true)
	{
		const int elementCount = split(str, key, stringBuffer, removeEmpty, showError);
		return elementCount == stringBuffer.maxSize();
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
	inline string ANSIToUTF8Auto(const char* str, bool addBOM = false) { return ANSIToUTF8(str, addBOM); }
	inline string ANSIToUTF8Auto(const string& str, bool addBOM = false) { return ANSIToUTF8(str, addBOM); }
#elif defined LINUX
	inline string ANSIToUTF8Auto(const char* str, bool addBOM = false) { return str; }
	inline const string& ANSIToUTF8Auto(const string& str, bool addBOM = false) { return str; }
#endif
	MICRO_LEGEND_FRAME_API string UTF8ToANSI(const char* str, bool eraseBOM = false);
	MICRO_LEGEND_FRAME_API string UTF8ToANSI(const string& str, bool eraseBOM = false);
	MICRO_LEGEND_FRAME_API void UTF8ToANSI(const char* str, char* output, int maxLength, bool eraseBOM = false);
#ifdef WINDOWS
	inline string UTF8ToANSIAuto(const char* str, bool eraseBOM = false) { return UTF8ToANSI(str, eraseBOM); }
	inline string UTF8ToANSIAuto(const string& str, bool eraseBOM = false) { return UTF8ToANSI(str, eraseBOM); }
#elif defined LINUX
	inline string UTF8ToANSIAuto(const char* str, bool eraseBOM = false) { return str; }
	inline const string& UTF8ToANSIAuto(const string& str, bool eraseBOM = false) { return str; }
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
	inline bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }
	inline bool isLower(char c) { return c >= 'a' && c <= 'z'; }
	inline bool isNumber(char c) { return c >= '0' && c <= '9'; }
	MICRO_LEGEND_FRAME_API bool isNumber(const string& str);
	inline bool isLetter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
	inline char toLower(const char value)
	{
		if (isUpper(value))
		{
			return value + 'a' - 'A';
		}
		return value;
	}
	inline char toUpper(const char value)
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
	inline bool checkIntString(const string& str, const string& valid = "") { return checkString(str, "0123456789" + valid); }
	inline bool checkFloatString(const string& str, const string& valid = "") { return checkIntString(str, '.' + valid); }
	MICRO_LEGEND_FRAME_API bool hasNonAscii(const char* str);
	MICRO_LEGEND_FRAME_API string byteToHexString(byte value, bool upper = true);
	MICRO_LEGEND_FRAME_API void byteToHexString(MyString<3>& byteHex, byte value, bool upper = true);
	MICRO_LEGEND_FRAME_API int getCharCount(const string& str, char key);
	MICRO_LEGEND_FRAME_API int getCharCount(const char* str, char key);
	MICRO_LEGEND_FRAME_API void appendWithAlign(string& oriStr, const string& appendStr, int alignWidth);
	MICRO_LEGEND_FRAME_API bool isPhoneNumber(const char* str);
	MICRO_LEGEND_FRAME_API string bytesToHexString(const byte* data, int dataCount, bool addSpace = true, bool upper = true);
	// data是指向一个可用内存的指针,长度为length,会将str转换为byte[]填充到data中
	MICRO_LEGEND_FRAME_API void hexStringToBytes(const string& str, byte* data, int length);
	inline byte hexCharToByte(const char hex)
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
	inline char byteToHexChar(const byte value)
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
	MICRO_LEGEND_FRAME_API bool hasControlChar(const string& str);
	// 字符串中是否包含控制字符,英文空格,中文空格
	MICRO_LEGEND_FRAME_API bool hasInvisibleChar(const string& str);
	// sql语法相关字符串处理
	// 带任何引号都不允许
	inline bool checkSQLString(const char* str) { return strchar(str, '\'') < 0 && strchar(str, '\"') < 0; }
	// 带任何引号都不允许
	inline bool checkSQLString(const string& str) { return strchar(str.c_str(), '\'', 0, (int)str.length()) < 0 && strchar(str.c_str(), '\"', 0, (int)str.length()) < 0; }
	// base64编码
	MICRO_LEGEND_FRAME_API string base64_encode(const byte* str, int length);
	MICRO_LEGEND_FRAME_API string base64_encode(const string& str);
	// base64解码
	MICRO_LEGEND_FRAME_API string base64_decode(const string& code);
	MICRO_LEGEND_FRAME_API string base64_decode(const char* code, int codeLen);
	// 计算一个字符串的SHA-1值
	MICRO_LEGEND_FRAME_API uint32_t rotate_left(uint32_t value, int shift);
	MICRO_LEGEND_FRAME_API Vector<uint8_t> pad_message(const string& message);
	MICRO_LEGEND_FRAME_API uint32_t bytes_to_uint32(const uint8_t* bytes);
	MICRO_LEGEND_FRAME_API void uint32_to_bytes(uint32_t value, uint8_t* bytes);
	MICRO_LEGEND_FRAME_API void sha1(const string& message, byte* buffer);
	MICRO_LEGEND_FRAME_API string sha1(const string& str);
}

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
using StringUtility::toLower;
using StringUtility::toUpper;
using StringUtility::checkString;
using StringUtility::getCharCount;
using StringUtility::POWER_INT_10;
using StringUtility::INVERSE_POWER_INT_10;
using StringUtility::POWER_LLONG_10;
using StringUtility::INVERSE_POWER_LLONG_10;
using StringUtility::V2IToS;
using StringUtility::V2IsToS;
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
using StringUtility::appendWithAlign;
using StringUtility::bytesToString;
using StringUtility::findStringLower;
using StringUtility::findStringPos;
using StringUtility::floatToStringExtra;
using StringUtility::getFileSuffix;
using StringUtility::getFirstFolderName;
using StringUtility::getLastChar;
using StringUtility::getLastNotNumberPos;
using StringUtility::getLastNumber;
using StringUtility::removeEndString;
using StringUtility::removeFirstPath;
using StringUtility::removeLast;
using StringUtility::removeStart;
using StringUtility::removeString;
using StringUtility::removeSuffix;
using StringUtility::strcat_s;
using StringUtility::strcpy_s;
using StringUtility::stringToULLong;
using StringUtility::V2SToS;
using StringUtility::V2USToS;
using StringUtility::zeroString;
using StringUtility::findCharCount;
using StringUtility::_itoa_s;
using StringUtility::greaterPower2;
using StringUtility::removeLastAll;
using StringUtility::removeStartAll;
using StringUtility::removeLastComma;
using StringUtility::getNotNumberSubString;
using StringUtility::_i64toa_s_;
using StringUtility::replaceSuffix;
using StringUtility::isPhoneNumber;
using StringUtility::byteToHexString;
using StringUtility::findStringInverse;
using StringUtility::checkSQLString;
using StringUtility::hexStringToBytes;
using StringUtility::jsonStartArray;
using StringUtility::jsonEndArray;
using StringUtility::jsonStartStruct;
using StringUtility::jsonEndStruct;
using StringUtility::jsonAddPair;
using StringUtility::jsonAddObject;