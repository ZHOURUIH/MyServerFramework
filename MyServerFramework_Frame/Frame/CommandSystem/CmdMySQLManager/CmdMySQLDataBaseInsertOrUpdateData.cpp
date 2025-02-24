#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseInsertOrUpdateData::execute()
{
	if (mData == nullptr || mTable == nullptr)
	{
		ERROR("Data或者Table为空!");
		return;
	}
	if (mData->isDestroy())
	{
		ERROR_PROFILE(("0数据已经被销毁,type:" + string(typeid(*mData).name())).c_str());
		return;
	}
	if (mTable->queryDataExistByID(mData->mID))
	{
		mTable->updateData(mData, FrameDefine::FULL_MYSQL_FLAG);
	}
	else
	{
		mTable->insertData(mData);
	}
	mMySQLDataPool->destroyClass(mData);
}
#endif