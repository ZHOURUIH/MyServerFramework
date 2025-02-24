#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"
#include "MySQLTableT.h"

template<typename T>
class CmdMySQLDataBaseQueryListByIDT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryListByIDT, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mValueList.clear();
		mCallback = nullptr;
	}
	void execute() override
	{
		// 去重
		removeDumplicate(mValueList);
		auto dataList = mTable->queryDataListByIDListT(move(mValueList));
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
	}
public:
	MySQLTableT<T>* mTable = nullptr;
	Vector<llong> mValueList;
	QueryDataListCallbackT<T> mCallback = nullptr;
};
#endif