#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseInsertData::execute()
{
	const bool success = mTable->insertData(mData, false);
	mMySQLDataPool->destroyClass(mData);
	mData = nullptr;
	if (mCallback != nullptr)
	{
		delayCall([success, callback = mCallback]() { callback(success); });
	}
}
#endif