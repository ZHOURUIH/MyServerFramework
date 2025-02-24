#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"
#include "MySQLDataScopeT.h"
#include "MySQLTableT.h"

template<typename T>
class CmdMySQLDataBaseQueryByColumnLLongT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryByColumnLLongT, GameCommand);
public:
	void reset() override
	{
		mColumns.clear();
		mTable = nullptr;
		mColumn = -1;
		mValue = 0;
		mCallback = nullptr;
	}
	void execute() override
	{
		auto data = mTable->queryDataByColumnLLongT(mColumn, mValue);
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, data = move(data)]() { CALL(callback, data.get()); });
	}
public:
	Vector<const char*> mColumns;
	MySQLTableT<T>* mTable = nullptr;
	int mColumn = -1;
	llong mValue = 0;
	QueryDataCallbackT<T> mCallback = nullptr;
};
#endif