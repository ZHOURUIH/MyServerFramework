#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseQueryExistByColumnString::execute()
{
	const bool exist = mTable->queryDataExistByColumnString(mColumn, mValue);
	// 延迟到主线程执行回调通知
	delayCall([callback = mCallback, exist]() { CALL(callback, exist); });
}
#endif