#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"
#include "MySQLTableT.h"

template<typename T>
class CmdMySQLDataBaseQueryListByColumnLLongT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryListByColumnLLongT, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mMaxCount = 0;
		mValue = 0;
		mCallback = nullptr;
	}
	void execute() override
	{
		auto dataList = mTable->queryDataListByColumnLLongT(mColumn, mValue, mMaxCount);
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
	}
public:
	MySQLTableT<T>* mTable = nullptr;
	int mColumn = -1;
	int mMaxCount = 0;
	llong mValue = 0;
	QueryDataListCallbackT<T> mCallback = nullptr;
};
#endif