#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseDeleteByColumnLLong2And::execute()
{
	mTable->deleteByColumnLLong2And(mColumn0, mValue0, mColumn1, mValue1, mOnlyOnce);
}
#endif