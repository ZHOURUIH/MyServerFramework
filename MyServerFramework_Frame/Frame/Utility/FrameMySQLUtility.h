#pragma once

#ifdef _MYSQL
#include "FrameCallback.h"
#include "FrameDefine.h"

namespace FrameMySQLUtility
{
	void MICRO_LEGEND_FRAME_API errorProfile(string&& log, string&& file);
	void MICRO_LEGEND_FRAME_API updateLLong(MySQLTable* table, llong id, int colName, llong value);
	void MICRO_LEGEND_FRAME_API updateLLongInt(MySQLTable* table, llong id, int colName0, llong value0, int colName1, int value1);
	void MICRO_LEGEND_FRAME_API updateBool(MySQLTable* table, llong id, int colName, bool value);
	void MICRO_LEGEND_FRAME_API updateInt(MySQLTable* table, llong id, int colName, int value);
	void MICRO_LEGEND_FRAME_API updateIntFloat(MySQLTable* table, llong id, int colName0, int value0, int colName1, float value1);
	void MICRO_LEGEND_FRAME_API updateInt2(MySQLTable* table, llong id, int colName0, int value0, int colName1, int value1);
	void MICRO_LEGEND_FRAME_API updateFloat(MySQLTable* table, llong id, int colName, float value);
	void MICRO_LEGEND_FRAME_API updateString(MySQLTable* table, llong id, int colName, const char* value, int length = -1);
	void MICRO_LEGEND_FRAME_API updateString2(MySQLTable* table, llong id, int colName0, const char* value0, int colName1, const char* value1);
	void MICRO_LEGEND_FRAME_API updateString(MySQLTable* table, llong id, int colName, string&& value);
	void MICRO_LEGEND_FRAME_API updateLLongList(MySQLTable* table, llong id, int colName, const Vector<llong>& value);
	void MICRO_LEGEND_FRAME_API updateAllInt(MySQLTable* table, int colName, int value);
	void MICRO_LEGEND_FRAME_API queryCountByColumnInt(MySQLTable* table, int colName, int value, const LLongCallback& callback);
	void MICRO_LEGEND_FRAME_API queryCountByColumnLLong(MySQLTable* table, int colName, llong value, const LLongCallback& callback);
	void MICRO_LEGEND_FRAME_API deleteDataByColumnLLong2And(MySQLTable* table, int colName0, llong value0, int colName1, llong value1, bool onlyOnce);
	void MICRO_LEGEND_FRAME_API deleteDataByColumnLLong(MySQLTable* table, int colName, llong value, bool onlyOnce);
	void MICRO_LEGEND_FRAME_API deleteDataByColumnString(MySQLTable* table, int colName, const char* value, bool onlyOnce);
	void MICRO_LEGEND_FRAME_API deleteDataByColumnString(MySQLTable* table, int colName, const string& value, bool onlyOnce);
	void MICRO_LEGEND_FRAME_API deleteDataByID(MySQLTable* table, llong id);
	void MICRO_LEGEND_FRAME_API deleteDataByIDList(MySQLTable* table, Vector<llong>&& idList);
	void MICRO_LEGEND_FRAME_API deleteDataCombine(MySQLTable* table0, MySQLTable* table1, llong id);
	void MICRO_LEGEND_FRAME_API deleteAll(MySQLTable* table);
	void MICRO_LEGEND_FRAME_API insertData(MySQLTable* table, MySQLData* data);
	void MICRO_LEGEND_FRAME_API insertDataCombine(MySQLTable* table0, MySQLTable* table1, MySQLData* data0, MySQLData* data1);
	void MICRO_LEGEND_FRAME_API insertOrUpdate(MySQLTable* table, MySQLData* data);
	void MICRO_LEGEND_FRAME_API pushCommandToMySQL(GameCommand* cmd);
	MICRO_LEGEND_FRAME_API extern ErrorProfileEvent mOnErrorProfileEvent;
};
#endif