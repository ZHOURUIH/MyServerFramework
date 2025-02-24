#ifdef _MYSQL
#include "FrameHeader.h"

void PreStatementWrite::destroy()
{
	if (mSTMT != nullptr)
	{
		mysql_stmt_close(mSTMT);
		mSTMT = nullptr;
	}
	ArrayPool::deleteArray(mParamBind, mParamCount);
	mParamCount = 0;
}

bool PreStatementWrite::init(MYSQL* mysql, const string& sql)
{
	mSTMT = mysql_stmt_init(mysql);
	if (mSTMT == nullptr)
	{
		return false;
	}
	if (mysql_stmt_prepare(mSTMT, sql.c_str(), (ulong)sql.length()))
	{
		ERROR("错误码:" + IToS(getError()) + ", sql:" + sql);
		return false;
	}
	mParamCount = (int)mysql_stmt_param_count(mSTMT);
	if (mParamCount > 0)
	{
		mParamBind = ArrayPool::newArray<MYSQL_BIND>(mParamCount);
	}
	return true;
}

bool PreStatementWrite::bindInt(const int index, int& value) const
{
	if (index >= mParamCount)
	{
		return false;
	}
	MYSQL_BIND& bind = mParamBind[index];
	bind.buffer_type = MYSQL_TYPE_LONG;
	bind.buffer = (void*)&value;
	bind.buffer_length = sizeof(value);
	return true;
}

bool PreStatementWrite::bindString(const int index, const string& value) const
{
	if (index >= mParamCount)
	{
		return false;
	}
	MYSQL_BIND& bind = mParamBind[index];
	bind.buffer_type = MYSQL_TYPE_STRING;
	bind.buffer = (void*)(value.c_str());
	bind.buffer_length = (ulong)value.length();
	return true;
}

bool PreStatementWrite::bindBlob(const int index, char* data, const int dataSize) const
{
	if (index >= mParamCount)
	{
		return false;
	}
	MYSQL_BIND& bind = mParamBind[index];
	bind.buffer_type = MYSQL_TYPE_BLOB;
	bind.buffer = (void*)data;
	bind.buffer_length = dataSize;
	return true;
}

bool PreStatementWrite::bindTime(const int index, MYSQL_TIME& value) const
{
	if (index >= mParamCount)
	{
		return false;
	}
	MYSQL_BIND& bind = mParamBind[index];
	bind.buffer_type = MYSQL_TYPE_TIMESTAMP;
	bind.buffer = (void*)&value;
	bind.buffer_length = sizeof(value);
	return true;
}

bool PreStatementWrite::execute() const
{
	if (mysql_stmt_bind_param(mSTMT, mParamBind) != 0)
	{
		ERROR("错误码:" + IToS(getError()));
		return false;
	}
	if (mysql_stmt_execute(mSTMT) != 0)
	{
		ERROR("错误码:" + IToS(getError()));
		return false;
	}
	if (mysql_stmt_affected_rows(mSTMT) == 0)
	{
		ERROR("错误码:" + IToS(getError()));
		return false;
	}
	return true;
}
#endif