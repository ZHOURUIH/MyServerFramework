#ifdef _MYSQL
#include "FrameHeader.h"

namespace FrameMySQLUtility
{
	ErrorProfileEvent mOnErrorProfileEvent;

	void errorProfile(string&& log, string&& file)
	{
		CALL(mOnErrorProfileEvent, move(log), move(file));
		// windows下发生错误就直接暂停运行
#ifdef WINDOWS
		system("pause");
#endif
	}

	void updateLLong(MySQLTable* table, const llong id, const int colName, const llong value)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateLLong, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn = colName;
		cmd->mValue = value;
		pushCommandToMySQL(cmd);
	}

	void updateLLongInt(MySQLTable* table, llong id, const int colName0, llong value0, const int colName1, int value1)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateLLongInt, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn0 = colName0;
		cmd->mColumn1 = colName1;
		cmd->mValue0 = value0;
		cmd->mValue1 = value1;
		pushCommandToMySQL(cmd);
	}

	void updateBool(MySQLTable* table, const llong id, const int colName, const bool value)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateBool, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn = colName;
		cmd->mValue = value;
		pushCommandToMySQL(cmd);
	}

	void updateInt(MySQLTable* table, const llong id, const int colName, const int value)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateInt, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn = colName;
		cmd->mValue = value;
		pushCommandToMySQL(cmd);
	}

	void updateIntFloat(MySQLTable* table, const llong id, const int colName0, const int value0, const int colName1, const float value1)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateIntFloat, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn0 = colName0;
		cmd->mValue0 = value0;
		cmd->mColumn1 = colName1;
		cmd->mValue1 = value1;
		pushCommandToMySQL(cmd);
	}

	void updateInt2(MySQLTable* table, const llong id, const int colName0, const int value0, const int colName1, const int value1)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateInt2, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn0 = colName0;
		cmd->mValue0 = value0;
		cmd->mColumn1 = colName1;
		cmd->mValue1 = value1;
		pushCommandToMySQL(cmd);
	}

	void updateFloat(MySQLTable* table, const llong id, const int colName, const float value)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateFloat, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn = colName;
		cmd->mValue = value;
		pushCommandToMySQL(cmd);
	}

	void updateString(MySQLTable* table, const llong id, const int colName, const char* value, const int length)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateString, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn = colName;
		cmd->mValue = value;
		if (length > 0 && length < (int)cmd->mValue.length())
		{
			cmd->mValue.erase(length);
		}
		pushCommandToMySQL(cmd);
	}

	void updateString2(MySQLTable* table, const llong id, const int colName0, const char* value0, const int colName1, const char* value1)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateString2, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn0 = colName0;
		cmd->mValue0 = value0;
		cmd->mColumn1 = colName1;
		cmd->mValue1 = value1;
		pushCommandToMySQL(cmd);
	}

	void updateString(MySQLTable* table, const llong id, const int colName, string&& value)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateString, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn = colName;
		cmd->mValue = move(value);
		pushCommandToMySQL(cmd);
	}

	void updateLLongList(MySQLTable* table, const llong id, const int colName, const Vector<llong>& value)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateLLongList, cmd);
		cmd->mTable = table;
		cmd->mID = id;
		cmd->mColumn = colName;
		value.clone(cmd->mValue);
		pushCommandToMySQL(cmd);
	}

	void updateAllInt(MySQLTable* table, const int colName, const int value)
	{
		CMD_THREAD(CmdMySQLDataBaseUpdateAllInt, cmd);
		cmd->mTable = table;
		cmd->mColumn = colName;
		cmd->mValue = value;
		pushCommandToMySQL(cmd);
	}

	void queryCountByColumnInt(MySQLTable* table, const int colName, const int value, const LLongCallback& callback)
	{
		CMD_THREAD(CmdMySQLDataBaseQueryCountByColumnInt, cmd);
		cmd->mTable = table;
		cmd->mColumn = colName;
		cmd->mValue = value;
		cmd->mCallback = callback;
		pushCommandToMySQL(cmd);
	}

	void queryCountByColumnLLong(MySQLTable* table, const int colName, const llong value, const LLongCallback& callback)
	{
		CMD_THREAD(CmdMySQLDataBaseQueryCountByColumnLLong, cmd);
		cmd->mTable = table;
		cmd->mColumn = colName;
		cmd->mValue = value;
		cmd->mCallback = callback;
		pushCommandToMySQL(cmd);
	}

	void deleteDataByColumnLLong2And(MySQLTable* table, const int colName0, const llong value0, const int colName1, const llong value1, const bool onlyOnce)
	{
		CMD_THREAD(CmdMySQLDataBaseDeleteByColumnLLong2And, cmd);
		cmd->mTable = table;
		cmd->mColumn0 = colName0;
		cmd->mColumn1 = colName1;
		cmd->mValue0 = value0;
		cmd->mValue1 = value1;
		cmd->mOnlyOnce = onlyOnce;
		pushCommandToMySQL(cmd);
	}

	void deleteDataByColumnLLong(MySQLTable* table, const int colName, const llong value, const bool onlyOnce)
	{
		CMD_THREAD(CmdMySQLDataBaseDeleteByColumnLLong, cmd);
		cmd->mTable = table;
		cmd->mColumn = colName;
		cmd->mValue = value;
		cmd->mOnlyOnce = onlyOnce;
		pushCommandToMySQL(cmd);
	}

	void deleteDataByColumnString(MySQLTable* table, const int colName, const char* value, const bool onlyOnce)
	{
		CMD_THREAD(CmdMySQLDataBaseDeleteByColumnString, cmd);
		cmd->mTable = table;
		cmd->mColumn = colName;
		cmd->mValue = value;
		cmd->mOnlyOnce = onlyOnce;
		pushCommandToMySQL(cmd);
	}

	void deleteDataByColumnString(MySQLTable* table, const int colName, const string& value, const bool onlyOnce)
	{
		CMD_THREAD(CmdMySQLDataBaseDeleteByColumnString, cmd);
		cmd->mTable = table;
		cmd->mColumn = colName;
		cmd->mValue = value;
		cmd->mOnlyOnce = onlyOnce;
		pushCommandToMySQL(cmd);
	}

	void deleteDataByID(MySQLTable* table, const llong id)
	{
		CMD_THREAD(CmdMySQLDataBaseDeleteByID, cmd);
		cmd->mTableList.push_back(table);
		cmd->mIDList.push_back(id);
		pushCommandToMySQL(cmd);
	}

	void deleteDataByIDList(MySQLTable* table, Vector<llong>&& idList)
	{
		if (idList.size() == 0)
		{
			return;
		}
		CMD_THREAD(CmdMySQLDataBaseDeleteByID, cmd);
		cmd->mTableList.push_back(table);
		idList.clone(cmd->mIDList);
		pushCommandToMySQL(cmd);
	}

	void deleteDataCombine(MySQLTable* table0, MySQLTable* table1, const llong id)
	{
		CMD_THREAD(CmdMySQLDataBaseDeleteByID, cmd);
		cmd->mTableList.push_back(table0, table1);
		cmd->mIDList.push_back(id);
		pushCommandToMySQL(cmd);
	}

	void deleteAll(MySQLTable* table)
	{
		CMD_THREAD(CmdMySQLDataBaseDeleteAll, cmd);
		cmd->mTable = table;
		pushCommandToMySQL(cmd);
	}

	// data需要是从对象池中创建的
	void insertData(MySQLTable* table, MySQLData* data)
	{
		CMD_THREAD(CmdMySQLDataBaseInsertData, cmd);
		cmd->mTable = table;
		cmd->mData = data;
		cmd->mCallback = nullptr;
		pushCommandToMySQL(cmd);
	}

	// data0,data1需要是从对象池中创建的
	void insertDataCombine(MySQLTable* table0, MySQLTable* table1, MySQLData* data0, MySQLData* data1)
	{
		CMD_THREAD(CmdMySQLDataBaseInsertCombine, cmd);
		cmd->mTable0 = table0;
		cmd->mTable1 = table1;
		cmd->mData0 = data0;
		cmd->mData1 = data1;
		pushCommandToMySQL(cmd);
	}

	// data需要是从对象池中创建的
	void insertOrUpdate(MySQLTable* table, MySQLData* data)
	{
		CMD_THREAD(CmdMySQLDataBaseInsertOrUpdateData, cmd);
		cmd->mTable = table;
		cmd->mData = data;
		pushCommandToMySQL(cmd);
	}

	void pushCommandToMySQL(GameCommand* cmd)
	{
		if (mMySQLManager->getThread() == nullptr)
		{
			ERROR("数据库线程还未创建,无法执行数据库命令");
		}
		mCommandSystem->pushCommandThread(cmd, mMySQLManager, mMySQLManager->getThread());
	}
}
#endif