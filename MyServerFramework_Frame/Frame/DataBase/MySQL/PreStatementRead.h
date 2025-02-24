#pragma once

#ifdef _MYSQL
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API PreStatementRead
{
public:
	virtual ~PreStatementRead() { destroy(); }
	void destroy();
	bool init(MYSQL* mysql, const string& sql);
	bool bindInt(const int index, int& value) const;
	bool bindString(const int index, const string& value) const;
	bool bindBlob(const int index, char* data, const int dataSize) const;
	bool bindTime(const int index, MYSQL_TIME& value) const;
	bool execute() const;
	bool query();
	bool setParamResult(const int index, const enum_field_types type, char* buffer, const ulong buffer_length, ulong* length) const;
	bool getResult() const { return mysql_stmt_bind_result(mSTMT, mResultBind) == 0 && mysql_stmt_store_result(mSTMT) == 0; }
	bool fetchResult() const { return mysql_stmt_fetch(mSTMT) == 0; }
protected:
	MYSQL_STMT* mSTMT = nullptr;
	MYSQL_BIND* mParamBind = nullptr;
	MYSQL_BIND* mResultBind = nullptr;
	MYSQL_RES* mResult = nullptr;
	int mParamCount = 0;
	int mResultCount = 0;
};
#endif