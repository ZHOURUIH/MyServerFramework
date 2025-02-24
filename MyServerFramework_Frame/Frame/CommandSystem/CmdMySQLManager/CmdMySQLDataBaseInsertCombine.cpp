#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseInsertCombine::execute()
{
	mTable0->insertData(mData0, false);
	mTable1->insertData(mData1, false);
	mMySQLDataPool->destroyClass(mData0);
	mMySQLDataPool->destroyClass(mData1);
}
#endif