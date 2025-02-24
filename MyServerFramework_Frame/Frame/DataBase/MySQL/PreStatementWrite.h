#pragma once

#ifdef _MYSQL
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API PreStatementWrite
{
public:
	PreStatementWrite();
	virtual ~PreStatementWrite() { destroy(); }
	void destroy();
	bool init(MYSQL* mysql, const string& sql);
	bool bindInt(const int index, int& value) const;
	bool bindString(const int index, const string& value) const;
	bool bindBlob(const int index, char* data, const int dataSize) const;
	bool bindTime(const int index, MYSQL_TIME& value) const;
	bool execute() const;
	int getError() const { return mysql_stmt_errno(mSTMT); }
protected:
	MYSQL_STMT* mSTMT = nullptr;
	MYSQL_BIND* mParamBind = nullptr;
	int mParamCount = 0;
};
#endif