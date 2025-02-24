#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseQueryCountByColumnInt::execute()
{
	MyString<64> condition;
	sqlConditionLLong(condition, mTable->getColName(mColumn), mValue);
	const llong dataCount = mTable->queryDataCount(condition.str());

	// 延迟到主线程执行回调通知
	delayCall([callback = mCallback, dataCount]() { CALL(callback, dataCount); });
}
#endif