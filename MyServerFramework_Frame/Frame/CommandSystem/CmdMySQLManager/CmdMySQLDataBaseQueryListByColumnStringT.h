#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"

template<typename T>
class CmdMySQLDataBaseQueryListByColumnStringT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryListByColumnStringT, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mValue.clear();
		mCallback = nullptr;
	}
	void execute() override
	{
		auto dataList = mTable->queryDataListByColumnStringT(mColumn, mValue);
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, dataList0 = move(dataList)]() { CALL(callback, dataList0.get()); });
	}
public:
	MySQLTableT<T>* mTable = nullptr;
	int mColumn = -1;
	string mValue;
	QueryDataListCallbackT<T> mCallback = nullptr;
};
#endif