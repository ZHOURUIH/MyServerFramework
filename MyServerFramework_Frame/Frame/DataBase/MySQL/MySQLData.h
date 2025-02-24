#pragma once

#ifdef _MYSQL
#include "ClassPooledObject.h"
#include "Utility.h"

class MICRO_LEGEND_FRAME_API MySQLData : public ClassPooledObject
{
	BASE(MySQLData, ClassPooledObject);
public:
	// 解析查询结果
	virtual void parseResult(const HashMap<int, char*>& resultRow) = 0;
	// 生成数据插入所需的字符串
	virtual void paramList(string& params) const = 0;
	// 生成更新所需的字符串,flag表示要更新的字段下标,最多允许64个字段
	virtual void generateUpdate(string& params, ullong flag) const = 0;
	virtual void clone(MySQLData* target) const { target->mID = mID; }
	// 克隆数据,flag表示要更新的字段下标,最多允许64个字段
	virtual void cloneWithFlag(MySQLData* target, ullong flag) const;
	// 基类不处理任何数据的更新,因为基类只有一个ID,而ID是不会被改变的
	virtual bool updateBool(bool value, int index) { return true; }
	virtual bool updateInt(int value, int index) { return true; }
	virtual bool updateFloat(float value, int index) { return true; }
	virtual bool updateLLong(llong value, int index) { return true; }
	virtual bool updateString(const string& value, int index) { return true; }
	virtual bool hasBool(bool value, int index) { return false; }
	virtual bool hasInt(int value, int index) { return false; }
	virtual bool hasFloat(float value, int index) { return false; }
	virtual bool hasLLong(llong value, int index) { return false; }
	virtual bool hasString(const string& value, int index) { return false; }
	virtual bool getBool(int index) { return false; }
	virtual int getInt(int index) { return 0; }
	virtual float getFloat(int index) { return 0.0f; }
	virtual llong getLLong(int index) { return 0; }
	virtual const string& getString(int index) { return FrameDefine::EMPTY; }
	// 此函数只是为了编译通过,并不会通过它来设置类型,而是在子类的构造中设置类型
	void setType(const char* type) {}
	static bool parseString(string& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr.assign(valueStr);
		return true;
	}
	static bool parseByte(byte& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToI(valueStr);
		return true;
	}
	static bool parseChar(char& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToI(valueStr);
		return true;
	}
	static bool parseUShort(ushort& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToI(valueStr);
		return true;
	}
	static bool parseShort(short& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToI(valueStr);
		return true;
	}
	static bool parseInt(int& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToI(valueStr);
		return true;
	}
	static bool parseUInt(uint& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToI(valueStr);
		return true;
	}
	static bool parseLLong(llong& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToLL(valueStr);
		return true;
	}
	static bool parseFloat(float& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToF(valueStr);
		return true;
	}
	static bool parseBool(bool& ptr, const char* valueStr)
	{
		if (valueStr == nullptr)
		{
			return false;
		}
		ptr = SToI(valueStr) > 0 || SToB(valueStr);
		return true;
	}
	void resetProperty() override;
public:
	llong mID = 0;						// 由于mysql语句生成的特殊性(主要是插入数据时候是否要添加逗号的问题),所以基类中并不会真正去操作ID,子类来操作
};
#endif