#ifdef _MYSQL
#include "FrameHeader.h"

void PreStatementRead::destroy()
{
	if (mSTMT != nullptr)
	{
		mysql_stmt_close(mSTMT);
		mSTMT = nullptr;
	}
	ArrayPool::deleteArray(mParamBind, mParamCount);
	ArrayPool::deleteArray(mResultBind, mResultCount);
	if (mResult != nullptr)
	{
		mysql_free_result(mResult);
		mResult = nullptr;
	}
	mParamCount = 0;
	mResultCount = 0;
}

bool PreStatementRead::init(MYSQL* mysql, const string& sql)
{
	mSTMT = mysql_stmt_init(mysql);
	if (mSTMT == nullptr)
	{
		return false;
	}
	if (mysql_stmt_prepare(mSTMT, sql.c_str(), (ulong)sql.length()))
	{
		return false;
	}
	mParamCount = (int)mysql_stmt_param_count(mSTMT);
	if (mParamCount > 0)
	{
		mParamBind = ArrayPool::newArray<MYSQL_BIND>(mParamCount);
	}
	return true;
}

bool PreStatementRead::bindInt(const int index, int& value) const
{
	if (index >= mParamCount)
	{
		return false;
	}
	MYSQL_BIND& bind = mParamBind[index];
	bind.buffer_type = MYSQL_TYPE_LONG;
	bind.buffer = (void*)&value;
	return true;
}

bool PreStatementRead::bindString(const int index, const string& value) const
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

bool PreStatementRead::bindBlob(const int index, char* data, const int dataSize) const
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

bool PreStatementRead::bindTime(const int index, MYSQL_TIME& value) const
{
	if (index >= mParamCount)
	{
		return false;
	}
	MYSQL_BIND& bind = mParamBind[index];
	bind.buffer_type = MYSQL_TYPE_TIMESTAMP;
	bind.buffer = &value;
	bind.buffer_length = sizeof(value);
	return true;
}

bool PreStatementRead::setParamResult(const int index, const enum_field_types type, char* buffer, const ulong buffer_length, ulong* length) const
{
	if (index >= mResultCount)
	{
		return false;
	}
	MYSQL_BIND& bind = mResultBind[index];
	bind.buffer_type = type;
	bind.buffer = buffer;
	bind.buffer_length = buffer_length;
	bind.length = length;
	return true;
}

bool PreStatementRead::execute() const
{
	if (mysql_stmt_bind_param(mSTMT, mParamBind) != 0)
	{
		return false;
	}
	if (mysql_stmt_execute(mSTMT) != 0)
	{
		return false;
	}
	if (mysql_stmt_affected_rows(mSTMT) == 0)
	{
		return false;
	}
	return true;
}

bool PreStatementRead::query()
{
	if (mysql_stmt_bind_param(mSTMT, mParamBind) != 0)
	{
		return false;
	}
	mResult = mysql_stmt_result_metadata(mSTMT);
	if (mResult == nullptr)
	{
		return false;
	}
	mResultCount = (int)mysql_num_fields(mResult);
	mResultBind = ArrayPool::newArray<MYSQL_BIND>(mResultCount);
	return mysql_stmt_execute(mSTMT) == 0;
}
#endif