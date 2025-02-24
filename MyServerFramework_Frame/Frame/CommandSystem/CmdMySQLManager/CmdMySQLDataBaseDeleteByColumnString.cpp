#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseDeleteByColumnString::execute()
{
	mTable->deleteByColumnString(mColumn, mValue, mOnlyOnce);
}
#endif