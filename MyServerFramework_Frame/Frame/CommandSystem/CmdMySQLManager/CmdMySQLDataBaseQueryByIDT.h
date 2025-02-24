#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"
#include "FrameUtility.h"
#include "MySQLTableT.h"

template<typename T>
class CmdMySQLDataBaseQueryByIDT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryByIDT, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mID = 0;
		mCallback = nullptr;
	}
	void execute() override
	{
		auto data = mTable->queryDataByIDT(mID);
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, data = move(data)]() { CALL(callback, data.get()); });
	}
public:
	MySQLTableT<T>* mTable = nullptr;
	llong mID = 0;
	QueryDataCallbackT<T> mCallback = nullptr;
};
#endif