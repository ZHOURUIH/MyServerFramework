#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"
#include "MySQLTableT.h"

template<typename T>
class CmdMySQLDataBaseQueryListByColumnStringListT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryListByColumnStringListT, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mValueList.clear();
		mCallback = nullptr;
	}
	void execute() override
	{
		auto dataList = mTable->queryDataListByColumnStringListT(mColumn, mValueList);
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
	}
public:
	MySQLTableT<T>* mTable = nullptr;
	int mColumn = -1;
	Vector<string> mValueList;
	QueryDataListCallbackT<T> mCallback = nullptr;
};
#endif