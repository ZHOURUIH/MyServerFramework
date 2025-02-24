#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"
#include "MySQLTableT.h"

template<typename T>
class CmdMySQLDataBaseQueryByColumnIntT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryByColumnIntT, GameCommand);
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
		// 先查询缓存
		auto data = mTable->queryDataByColumnIntT(mColumn, mValue);
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, data]() { CALL(callback, data.get()); });
	}
public:
	Vector<const char*> mColumns;
	MySQLTableT<T>* mTable = nullptr;
	int mColumn = -1;
	int mValue = 0;
	QueryDataCallbackT<T> mCallback = nullptr;
};
#endif