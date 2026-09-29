#pragma once

#include "StringUtility.h"

namespace SQLUtility
{
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
			queryStr.add("\"", str, addComma ? "\"," : "\"");
		}
		else
		{
			queryStr.add("\"", addComma ? "\"," : "\"");
		}
	}
	inline void sqlAddString(string& queryStr, const string& str, const bool addComma)
	{
		if (checkSQLString(str))
		{
			queryStr += '\"';
			queryStr += str;
			queryStr += addComma ? "\"," : "\"";
		}
		else
		{
			queryStr += '\"';
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
				queryStr.add("\"", utf8String, addComma ? "\"," : "\"");
			}
			else
			{
				queryStr.add("\"", addComma ? "\"," : "\"");
			}
		}
		else
		{
			queryStr.add("\"", addComma ? "\"," : "\"");
		}
	}
	MICRO_LEGEND_FRAME_API void sqlAddInt(char* queryStr, int size, int value, bool addComma = true);
	inline void sqlAddInt(string& queryStr, const int value, const bool addComma = true)
	{
		INT_STR(temp, value);
		queryStr += temp.str();
		if (addComma)
		{
			queryStr += ',';
		}
	}
	template<int Length>
	void sqlAddInt(MyString<Length>& queryStr, const int value, const bool addComma = true)
	{
		INT_STR(temp, value);
		if (addComma)
		{
			queryStr.add(temp.str(), ",");
		}
		else
		{
			queryStr.add(temp.str());
		}
	}
	MICRO_LEGEND_FRAME_API void sqlAddUInt(char* queryStr, int size, int value, bool addComma = true);
	template<int Length>
	void sqlAddUInt(MyString<Length>& queryStr, const int value, const bool addComma = true)
	{
		UINT_STR(temp, value);
		if (addComma)
		{
			queryStr.add(temp.str(), ",");
		}
		else
		{
			queryStr.add(temp.str());
		}
	}
	MICRO_LEGEND_FRAME_API void sqlAddFloat(char* queryStr, int size, float value, bool addComma = true);
	inline void sqlAddFloat(string& queryStr, const float value, const bool addComma = true)
	{
		FLOAT_STR(temp, value);
		queryStr += temp.str();
		if (addComma)
		{
			queryStr += ',';
		}
	}
	template<int Length>
	void sqlAddFloat(MyString<Length>& queryStr, const float value, const bool addComma = true)
	{
		FLOAT_STR(temp, value);
		if (addComma)
		{
			queryStr.add(temp.str(), ",");
		}
		else
		{
			queryStr.add(temp.str());
		}
	}
	MICRO_LEGEND_FRAME_API void sqlAddULLong(char* queryStr, int size, ullong value, bool addComma = true);
	template<int Length>
	void sqlAddULLong(MyString<Length>& queryStr, const ullong value, const bool addComma = true)
	{
		ULLONG_STR(temp, value);
		if (addComma)
		{
			queryStr.add(temp.str(), ",");
		}
		else
		{
			queryStr.add(temp.str());
		}
	}
	MICRO_LEGEND_FRAME_API void sqlAddLLong(char* queryStr, int size, llong value, bool addComma = true);
	template<int Length>
	void sqlAddLLong(MyString<Length>& queryStr, const llong value, const bool addComma = true)
	{
		LLONG_STR(temp, value);
		if (addComma)
		{
			queryStr.add(temp.str(), ",");
		}
		else
		{
			queryStr.add(temp.str());
		}
	}
	inline void sqlAddLLong(string& queryStr, const llong value, const bool addComma = true)
	{
		LLONG_STR(temp, value);
		queryStr += temp.str();
		if (addComma)
		{
			queryStr += ',';
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
	inline void sqlAddBytes(string& queryStr, const Vector<byte>& byteArray, const bool addComma = true)
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
	inline void sqlAddUShorts(string& queryStr, const Vector<ushort>& ushortArray, const bool addComma = true)
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
	inline void sqlAddInts(string& queryStr, const Vector<int>& intArray, const bool addComma = true)
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
	inline void sqlAddUInts(string& queryStr, const Vector<uint>& intArray, bool addComma = true)
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
	inline void sqlAddFloats(string& queryStr, const Vector<float>& floatArray, const bool addComma = true)
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
	inline void sqlAddULLongs(string& queryStr, const Vector<ullong>& longArray, const bool addComma = true)
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
	inline void sqlAddLLongs(string& queryStr, const Vector<llong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		LLsToS(charArray.mArray, arrayLen, longArray);
		sqlAddString(queryStr, charArray.mArray, addComma);
	}
	MICRO_LEGEND_FRAME_API void sqlAddVector2Int(char* queryStr, int size, Vector2Int value, bool addComma = true);
	template<int Length>
	void sqlAddVector2Int(MyString<Length>& queryStr, Vector2Int value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	inline void sqlAddVector2Int(string& queryStr, Vector2Int value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	MICRO_LEGEND_FRAME_API void sqlAddVector2UInt(char* queryStr, int size, Vector2UInt value, bool addComma = true);
	template<int Length>
	void sqlAddVector2UInt(MyString<Length>& queryStr, Vector2UInt value, const bool addComma = true)
	{
		MyString<16> temp;
		V2UIToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	inline void sqlAddVector2UInt(string& queryStr, Vector2UInt value, const bool addComma = true)
	{
		MyString<16> temp;
		V2UIToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	MICRO_LEGEND_FRAME_API void sqlAddVector2Short(char* queryStr, int size, Vector2Short value, bool addComma = true);
	template<int Length>
	void sqlAddVector2Short(MyString<Length>& queryStr, Vector2Short value, const bool addComma = true)
	{
		MyString<16> temp;
		V2SToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	inline void sqlAddVector2Short(string& queryStr, Vector2Short value, const bool addComma = true)
	{
		MyString<16> temp;
		V2SToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	MICRO_LEGEND_FRAME_API void sqlAddVector2UShort(char* queryStr, int size, Vector2UShort value, bool addComma = true);
	template<int Length>
	void sqlAddVector2UShort(MyString<Length>& queryStr, Vector2UShort value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	inline void sqlAddVector2UShort(string& queryStr, Vector2UShort value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlAddString(queryStr, temp.str(), addComma);
	}
	// where条件相关
	inline void sqlConditionString(string& condition, const string& col, const char* str)
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
	inline void sqlConditionString(string& condition, const string& col, const string& str)
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
			condition.add(col.c_str(), "=\"", str, "\"");
		}
		else
		{
			condition.add(col.c_str(), "=\"", "\"");
		}
	}
	template<int Length>
	void sqlConditionString(MyString<Length>& condition, const string& col, const char* str, const char* relationalOperator, const char* logicalOperator)
	{
		if (checkSQLString(str))
		{
			condition.add(col.c_str(), relationalOperator, "\"", str, "\"", logicalOperator);
		}
		else
		{
			condition.add(col.c_str(), relationalOperator, "\"\"", logicalOperator);
		}
	}
	inline void sqlConditionString(string& condition, const string& col, const char* str, const char* relationalOperator, const char* logicalOperator)
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
	inline void sqlConditionString(string& condition, const string& col, const string& str, const char* relationalOperator, const char* logicalOperator)
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
			condition.add(col.c_str(), " LIKE \"%", str, "%\"");
		}
		else
		{
			// 此处不好处理,如果条件带引号,直接去除的话,会导致查询全表
			condition.add(col.c_str(), " = \"\"");
		}
	}
	template<int Length>
	void sqlConditionStringLike(MyString<Length>& condition, const string& col, const char* str, const char* logicalOperator)
	{
		if (checkSQLString(str))
		{
			condition.add(col.c_str(), " LIKE \"%", str, "%\"", logicalOperator);
		}
		else
		{
			condition.add(col.c_str(), " = \"\"", logicalOperator);
		}
	}
	template<int Length>
	void sqlConditionStringLike(MyString<Length>& condition, const string& col, const char* str, const char* logicalOperator, const char* prev, const char* end)
	{
		if (checkSQLString(str))
		{
			condition.add(col.c_str(), " LIKE \"", prev, str, end, "\"", logicalOperator);
		}
		else
		{
			condition.add(col.c_str(), " = \"\"", logicalOperator);
		}
	}
	inline void sqlConditionStringLike(string& condition, const string& col, const char* str)
	{
		if (checkSQLString(str))
		{
			condition += col;
			condition += " LIKE \"%";
			condition += str;
			condition += "%\"";
		}
		else
		{
			// 此处不好处理,如果条件带引号,直接去除的话,会导致查询全表
			condition += col;
			condition += " = \"\"";
		}
	}
	inline void sqlConditionStringLike(string& condition, const string& col, const char* str, const char* logicalOperator)
	{
		if (checkSQLString(str))
		{
			condition += col;
			condition += " LIKE \"%";
			condition += str;
			condition += "%\"";
			condition += logicalOperator;
		}
		else
		{
			condition += col;
			condition += " = \"\"";
			condition += logicalOperator;
		}
	}
	inline void sqlConditionStringLike(string& condition, const string& col, const char* str, const char* logicalOperator, const char* prev, const char* end)
	{
		if (checkSQLString(str))
		{
			condition += col;
			condition += " LIKE \"";
			condition += prev;
			condition += str;
			condition += end;
			condition += "\"";
			condition += logicalOperator;
		}
		else
		{
			condition += col;
			condition += " = \"\"";
			condition += logicalOperator;
		}
	}
	template<int Length>
	void sqlConditionInt(MyString<Length>& condition, const string& col, const int value)
	{
		INT_STR(temp, value);
		condition.add(col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionInt(MyString<Length>& condition, const string& col, const int value, const char* relationalOperator)
	{
		INT_STR(temp, value);
		condition.add(col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionInt(MyString<Length>& condition, const string& col, const int value, const char* relationalOperator, const char* logicalOperator)
	{
		INT_STR(temp, value);
		condition.add(col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	inline void sqlConditionInt(string& condition, const string& col, const int value)
	{
		INT_STR(temp, value);
		condition += col;
		condition += "=";
		condition += temp.str();
	}
	inline void sqlConditionInt(string& condition, const string& col, const int value, const char* relationalOperator)
	{
		INT_STR(temp, value);
		condition += col;
		condition += relationalOperator;
		condition += temp.str();
	}
	inline void sqlConditionInt(string& condition, const string& col, const int value, const char* relationalOperator, const char* logicalOperator)
	{
		INT_STR(temp, value);
		condition += col;
		condition += relationalOperator;
		condition += temp.str();
		condition += logicalOperator;
	}
	template<int Length>
	void sqlConditionFloat(MyString<Length>& condition, const string& col, const float value)
	{
		FLOAT_STR(temp, value);
		condition.add(col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionFloat(MyString<Length>& condition, const string& col, const float value, const char* relationalOperator)
	{
		FLOAT_STR(temp, value);
		condition.add(col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionFloat(MyString<Length>& condition, const string& col, const float value, const char* relationalOperator, const char* logicalOperator)
	{
		FLOAT_STR(temp, value);
		condition.add(col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	inline void sqlConditionFloat(string& condition, const string& col, const float value)
	{
		FLOAT_STR(temp, value);
		condition += col;
		condition += "=";
		condition += temp.str();
	}
	inline void sqlConditionFloat(string& condition, const string& col, const float value, const char* relationalOperator)
	{
		FLOAT_STR(temp, value);
		condition += col;
		condition += relationalOperator;
		condition += temp.str();
	}
	inline void sqlConditionFloat(string& condition, const string& col, const float value, const char* relationalOperator, const char* logicalOperator)
	{
		FLOAT_STR(temp, value);
		condition += col;
		condition += relationalOperator;
		condition += temp.str();
		condition += logicalOperator;
	}
	template<int Length>
	void sqlConditionLLong(MyString<Length>& condition, const string& col, const llong value)
	{
		LLONG_STR(temp, value);
		condition.add(col.c_str(), "=", temp.str());
	}
	template<int Length>
	void sqlConditionLLong(MyString<Length>& condition, const string& col, const llong value, const char* relationalOperator)
	{
		LLONG_STR(temp, value);
		condition.add(col.c_str(), relationalOperator, temp.str());
	}
	template<int Length>
	void sqlConditionLLong(MyString<Length>& condition, const string& col, const llong value, const char* relationalOperator, const char* logicalOperator)
	{
		LLONG_STR(temp, value);
		condition.add(col.c_str(), relationalOperator, temp.str(), logicalOperator);
	}
	inline void sqlConditionLLong(string& condition, const string& col, const llong value)
	{
		condition.reserve(condition.size() + col.length() + 10);
		LLONG_STR(temp, value);
		condition += col;
		condition += "=";
		condition += temp.str();
	}
	inline void sqlConditionLLong(string& condition, const string& col, const llong value, const char* relationalOperator)
	{
		condition.reserve(condition.size() + col.length() + 16);
		LLONG_STR(temp, value);
		condition += col;
		condition += relationalOperator;
		condition += temp.str();
	}
	inline void sqlConditionLLong(string& condition, const string& col, const llong value, const char* relationalOperator, const char* logicalOperator)
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
			updateInfo.add(col.c_str(), " = \"", utf8String, addComma ? "\"," : "\"");
		}
		else
		{
			updateInfo.add(col.c_str(), " = \"", addComma ? "\"," : "\"");
		}
	}
	inline void sqlUpdateString(string& updateInfo, const string& col, const char* str, const bool addComma = true)
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
			updateInfo.add(col.c_str(), " = \"", str, addComma ? "\"," : "\"");
		}
		else
		{
			updateInfo.add(col.c_str(), " = \"", addComma ? "\"," : "\"");
		}
	}
	template<int Length>
	void sqlUpdateString(MyString<Length>& updateInfo, const string& col, const char* str, const int strLength, const bool addComma = true)
	{
		if (checkSQLString(str))
		{
			updateInfo.add(col.c_str(), " = \"");
			updateInfo.add(str, strLength);
			updateInfo.add(addComma ? "\"," : "\"");
		}
		else
		{
			updateInfo.add(col.c_str(), " = \"", addComma ? "\"," : "\"");
		}
	}
	inline void sqlUpdateString(string& updateInfo, const string& col, const string& str, const bool addComma = true)
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
			updateInfo.add(col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			updateInfo.add(col.c_str(), " = ", temp.str());
		}
	}
	inline void sqlUpdateBool(string& updateInfo, const string& col, const bool value, const bool addComma = true)
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
			updateInfo.add(col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			updateInfo.add(col.c_str(), " = ", temp.str());
		}
	}
	inline void sqlUpdateInt(string& updateInfo, const string& col, const int value, const bool addComma = true)
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
	void sqlUpdateFloat(MyString<Length>& updateInfo, const string& col, const float value, const bool addComma = true)
	{
		FLOAT_STR(temp, value);
		if (addComma)
		{
			updateInfo.add(col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			updateInfo.add(col.c_str(), " = ", temp.str());
		}
	}
	inline void sqlUpdateFloat(string& updateInfo, const string& col, const float value, const bool addComma = true)
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
	void sqlUpdateLLong(MyString<Length>& updateInfo, const string& col, const llong value, const bool addComma = true)
	{
		LLONG_STR(temp, value);
		if (addComma)
		{
			updateInfo.add(col.c_str(), " = ", temp.str(), ",");
		}
		else
		{
			updateInfo.add(col.c_str(), " = ", temp.str());
		}
	}
	inline void sqlUpdateLLong(string& updateInfo, const string& col, const llong value, const bool addComma = true)
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
	inline void sqlUpdateBytes(string& updateInfo, const string& col, const Vector<byte>& byteArray, const bool addComma = true)
	{
		const int arrayLen = 4 * greaterPower2(byteArray.size());
		CharArrayScopeThread charArray(arrayLen);
		bytesToString(charArray.mArray, arrayLen, byteArray);
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
	inline void sqlUpdateInts(string& updateInfo, const string& col, const Vector<int>& intArray, bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(intArray.size());
		CharArrayScopeThread charArray(arrayLen);
		IsToS(charArray.mArray, arrayLen, intArray);
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
	inline void sqlUpdateFloats(string& updateInfo, const string& col, const Vector<float>& floatArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(floatArray.size());
		CharArrayScopeThread charArray(arrayLen);
		FsToS(charArray.mArray, arrayLen, floatArray);
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
	inline void sqlUpdateLLongs(string& updateInfo, const string& col, const Vector<llong>& longArray, const bool addComma = true)
	{
		const int arrayLen = 16 * greaterPower2(longArray.size());
		CharArrayScopeThread charArray(arrayLen);
		LLsToS(charArray.mArray, arrayLen, longArray);
		sqlUpdateString(updateInfo, col, charArray.mArray, addComma);
	}
	template<int Length>
	void sqlUpdateVector2Int(MyString<Length>& updateInfo, const string& col, Vector2Int value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	inline void sqlUpdateVector2Int(string& updateInfo, const string& col, Vector2Int value, const bool addComma = true)
	{
		MyString<32> temp;
		V2IToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	template<int Length>
	void sqlUpdateVector2UInt(MyString<Length>& updateInfo, const string& col, Vector2UInt value, const bool addComma = true)
	{
		MyString<32> temp;
		V2UIToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	inline void sqlUpdateVector2UInt(string& updateInfo, const string& col, Vector2UInt value, const bool addComma = true)
	{
		MyString<32> temp;
		V2UIToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	template<int Length>
	void sqlUpdateVector2UShort(MyString<Length>& updateInfo, const string& col, Vector2UShort value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
	inline void sqlUpdateVector2UShort(string& updateInfo, const string& col, Vector2UShort value, const bool addComma = true)
	{
		MyString<16> temp;
		V2USToS(temp, value);
		sqlUpdateString(updateInfo, col, temp.str(), addComma);
	}
}

using SQLUtility::sqlConditionInt;
using SQLUtility::sqlConditionFloat;
using SQLUtility::sqlConditionString;
using SQLUtility::sqlConditionLLong;
using SQLUtility::sqlConditionStringLike;
using SQLUtility::sqlUpdateInt;
using SQLUtility::sqlUpdateString;
using SQLUtility::sqlUpdateLLong;
using SQLUtility::sqlUpdateBool;
using SQLUtility::sqlUpdateFloat;
using SQLUtility::sqlAddInt;
using SQLUtility::sqlAddFloat;
using SQLUtility::sqlAddLLong;
using SQLUtility::sqlAddString;
using SQLUtility::sqlAddStringUTF8;
using SQLUtility::sqlUpdateStringUTF8;