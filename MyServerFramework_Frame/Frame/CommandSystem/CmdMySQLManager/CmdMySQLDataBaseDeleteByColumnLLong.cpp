#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseDeleteByColumnLLong::execute()
{
	// 第0列固定为ID
	if (mColumn == 0)
	{
		mTable->deleteByID(mValue);
	}
	else
	{
		mTable->deleteByColumnLLong(mColumn, mValue, mOnlyOnce);
	}
}
#endif