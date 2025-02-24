#ifdef _MYSQL
#include "FrameHeader.h"

void CmdMySQLDataBaseDeleteAll::execute()
{
	mTable->deleteAll();
}
#endif