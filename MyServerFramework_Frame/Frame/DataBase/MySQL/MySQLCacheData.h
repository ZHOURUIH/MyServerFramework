#pragma once

#ifdef _MYSQL
#include "ClassPooledObject.h"
#include "FrameBase.h"
#include "MySQLData.h"
#include "MySQLTable.h"

class MICRO_LEGEND_FRAME_API MySQLCacheData : public ClassPooledObject
{
	BASE(MySQLCacheData, ClassPooledObject);
public:
	void setTable(MySQLTable* table);
	void destroy();
	void resetProperty() override;
	llong getID() const { return mData->mID; }
	MySQLData* getData() const { return mData; }
	// 因为数据不会从有效变为失效的情况(除非是绕过服务器单独修改了数据库数据,否则已经查询过的缓存数据一定与数据库的一致)
	// 所以在设置标记时只会增加标记,而不会减少标记
	void addValidFlag(const ullong flag) { mValidFlag |= flag; }
	ullong getValidFlag() const { return mValidFlag; }
	void updateBool(const int index, const bool value)
	{
		if (!mData->updateBool(value, index))
		{
			ERROR("更新缓存数据类型错误");
		}
		setColumnValid(index);
	}
	void updateInt(const int index, const int value)
	{
		if (!mData->updateInt(value, index))
		{
			ERROR("更新缓存数据类型错误");
		}
		setColumnValid(index);
	}
	void updateFloat(const int index, const float value)
	{
		if (!mData->updateFloat(value, index))
		{
			ERROR("更新缓存数据类型错误");
		}
		setColumnValid(index);
	}
	void updateLLong(const int index, const llong value)
	{
		if (!mData->updateLLong(value, index))
		{
			ERROR("更新缓存数据类型错误");
		}
		setColumnValid(index);
	}
	void updateString(const int index, const string& value)
	{
		if (!mData->updateString(value, index))
		{
			ERROR("更新缓存数据类型错误");
		}
		setColumnValid(index);
	}
	void setColumnValid(int index) { setBitOne(mValidFlag, index); }
	bool isColumnValid(int index) const { return hasBit(mValidFlag, index); }
	bool isColumnNameValid(const int colName) const { return colName >= 0 && isColumnValid(colName); }
	bool isColumnNamesValid(const Vector<int>& colNames) const;
	bool hasBool(const int column, const bool value) const { return mData->hasBool(value, column); }
	bool hasInt(const int column, const int value) const { return mData->hasInt(value, column); }
	bool hasFloat(const int column, const float value) const { return mData->hasFloat(value, column); }
	bool hasLLong(const int column, const llong value) const { return mData->hasLLong(value, column); }
	bool hasString(const int column, const string& value) const { return mData->hasString(value, column); }
protected:
	MySQLTable* mTable = nullptr;		// MySQL数据所属的表格
	MySQLData* mData = nullptr;			// 数据对象
	ullong mValidFlag = 0;				// 所有字段的有效性标记
};

#endif