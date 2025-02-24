#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"

template<typename T>
class CmdMySQLDataBaseQueryByColumnStringT : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryByColumnStringT, GameCommand);
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
		auto data = mTable->queryDataByColumnStringT(mColumn, mValue);
		// 延迟到主线程执行回调通知
		delayCall([callback = mCallback, data = move(data)]() { CALL(callback, data.get()); });
	}
public:
	MySQLTableT<T>* mTable = nullptr;
	int mColumn = -1;
	string mValue;
	QueryDataCallbackT<T> mCallback = nullptr;
};
#endif