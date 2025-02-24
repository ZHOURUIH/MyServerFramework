#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"
#include "MySQLTableT.h"

template<typename T>
class CmdMySQLDataBaseInsertDataT : public GameCommand
{
	BASE(CmdMySQLDataBaseInsertDataT, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mData = nullptr;
		mCallback = nullptr;
	}
	void execute() override
	{
		const llong id = mData->mID;
		const bool success = mTable->insertData(mData, false);
		mMySQLDataPool->destroyClass(mData);
		mData = nullptr;
		if (mCallback != nullptr)
		{
			if (success)
			{
				auto data = mTable->queryDataByIDT(id);
				delayCall([data = move(data), callback = mCallback]() { callback(data.get()); });
			}
			else
			{
				delayCall([callback = mCallback]() { callback(nullptr); });
			}
		}
	}
public:
	MySQLTableT<T>* mTable = nullptr;
	MySQLData* mData = nullptr;
	QueryDataCallbackT<T> mCallback = nullptr;
};
#endif