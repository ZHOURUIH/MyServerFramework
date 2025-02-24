#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseQueryCountByColumnLLong2Or::execute()
{
	MyString<128> condition;
	sqlConditionLLong(condition, mTable->getColName(mColumn0), mValue1, "=", "||");
	sqlConditionLLong(condition, mTable->getColName(mColumn0), mValue1);
	const llong dataCount = mTable->queryDataCount(condition.str());

	// 延迟到主线程执行回调通知
	delayCall([callback = mCallback, dataCount]() { CALL(callback, dataCount); });
}
#endif